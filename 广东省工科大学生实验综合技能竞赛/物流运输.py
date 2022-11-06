from machine import Timer,PWM,UART,I2C
from anjian import qidong2,qidong
from duoji import open1,close1,open2,close2
import sensor,time
from Maix import GPIO
from fpioa_manager import fm
from ssd1306k import SSD1306

#映射串口引脚
fm.register(6, fm.fpioa.UART1_RX, force=True)
fm.register(7, fm.fpioa.UART1_TX, force=True)
fm.register(16, fm.fpioa.GPIO1)

fm.register(34, fm.fpioa.GPIO2)
fm.register(33, fm.fpioa.GPIO3)
fm.register(15, fm.fpioa.GPIO4)
fm.register(8, fm.fpioa.GPIO5)
fm.register(9, fm.fpioa.GPIO6)
fm.register(32, fm.fpioa.GPIO7)

fm.register(30, fm.fpioa.GPIO0)
#fm.register(31, fm.fpioa.GPIO8)

KEY = GPIO(GPIO.GPIO1, GPIO.IN)
KEY1 = GPIO(GPIO.GPIO2, GPIO.OUT)
KEY2 = GPIO(GPIO.GPIO3, GPIO.OUT)
KEY3 = GPIO(GPIO.GPIO4, GPIO.OUT)
KEY4 = GPIO(GPIO.GPIO5, GPIO.OUT)
KEY5 = GPIO(GPIO.GPIO6, GPIO.OUT)
KEY6 = GPIO(GPIO.GPIO7, GPIO.OUT)

KEY7 = GPIO(GPIO.GPIO0, GPIO.OUT)  #待用

KEY1.value(1)
KEY2.value(1)
KEY3.value(1)
KEY4.value(1)
KEY5.value(1)
KEY6.value(1)
KEY7.value(1)
#初始化
i2c = I2C(I2C.I2C0, mode=I2C.MODE_MASTER,scl=27, sda=28)
oled = SSD1306(i2c, addr=0x3c)
uart = UART(UART.UART1, 9600, read_buf_len=4096)
#摄像头模块初始化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_vflip(1)     #后置模式(图像反转，所见即所得)   0取消反转
sensor.skip_frames(5)   #摄像头配置后跳过 n 帧或者等待时间 time 让其变稳定。
                         #n:跳过帧数；time：等待 时间,单位 ms。 （如果 n 和 time 均没指定，则默认跳过 300 毫秒的帧。）
thresholds = [(55, 100, 18, 127, -8, 127), # 红色阈值
              (19, 70, -43, -7, -20, 32), # 绿色阈值(0, 46, -35, -12, -128, 127)
              (0, 30, 0, 64, -128, -20)] # 蓝色阈值
str1 = '12'
str2 = '21'
str3 = '11'
str4 = '22'
m=0
t=0
x=0
def fun(tim):
    step = str(t)
    #清屏,0x00(白屏)，0xff(黑屏)
    oled.fill(0)    #显示字符。参数格式为（str,x,y）,其中x范围是0-127，y范围是0-7（共8行）
    oled.text("step", 0, 0) #写入第 0 行内容
    oled.text(step, 50, 0) #写入第 0 行内容
    oled.text("Hello World!", 0, 2) #写入第 1 行内容
    oled.text("MicroPython", 0, 4) #写入第 2 行内容
    oled.text("By 01Studio", 0, 6) #写入第 5 行内容
#定时器0初始化，周期1秒
tim = Timer(Timer.TIMER2, Timer.CHANNEL2, mode=Timer.MODE_PERIODIC, period=1000,unit=Timer.UNIT_MS, callback=fun)

clock = time.clock()
while(True):
    if t==0:
        clock.tick() #开始追踪运行时间
        img = sensor.snapshot()
        res = img.find_qrcodes() #寻找二维码
        if len(res) > 0: #在图片和终端显示二维码信息
            img.draw_rectangle(res[0].rect())
            img.draw_string(2,2, res[0].payload(), color=(0,128,0), scale=2)
            #print(res[0].payload())
            m=res[0].payload()
            if m==str1:
                data12 = bytearray([0x12])
                print('识别到12')
                open1()
                KEY1.value(0)
                time.sleep(2)
                KEY1.value(1)
                close1()
                uart.write(data12)
                m=0
                t=1
            if m==str2:
                data21 = bytearray([0x21])
                print('识别到21')
                open2()
                KEY2.value(0)
                time.sleep(2)
                KEY2.value(1)
                close2()
                uart.write(data21)
                m=0
                t=1
            if m==str3:
                data11 = bytearray([0x11])
                print('识别到11')
                open1()
                KEY1.value(0)
                time.sleep(2)
                KEY1.value(1)
                close1()
                uart.write(data11)
                m=0
                t=1
            if m==str4:
                data22 = bytearray([0x22])
                print('识别到22')
                open2()
                KEY2.value(0)
                time.sleep(2)
                KEY2.value(1)
                close2()
                uart.write(data22)
                m=0
                t=1
    if t==1:
        while(x<18):
            qidong()
            data = bytearray([0x4B])
            data1 = bytearray([0x5A])
            uart.write(data1)
            uart.write(data)
            x += 1
        #while (t==1):
        img=sensor.snapshot()
        #left_roi = [50,30,60,60]  #设置感兴趣区域
        for blob in img.find_blobs(thresholds,pixels_threshold=500,merge = True): # 0,1,2分别表示红，绿，蓝色。
        #image.find_blobs(thresholds, roi=Auto, x_stride=1, y_stride=1, invert=False, area_threshold=10, pixels_threshold=10, merge=False, margin=0, threshold_cb=None, merge_cb=None)
            if blob.code() == 2:
                #tmp=img.draw_circle(blob[5],blob[6],30)
                #tmp=img.draw_cross(blob[5], blob[6])
                print("识别到绿灯")
                qidong2()
                t=2
    if t==2:
        while(t==2):
            if uart.any():
                c=uart.readline().decode()
                print(c)
                if c=='1':
                    KEY3.value(0)
                    time.sleep(1)
                    KEY3.value(1)
                    t=3
                if c=='2':
                    KEY4.value(0)
                    time.sleep(1)
                    KEY4.value(1)
                    t=3
    if t==3:
        img = sensor.snapshot()
        res = img.find_qrcodes() #寻找二维码
        if len(res) > 0: #在图片和终端显示二维码信息
            img.draw_rectangle(res[0].rect())
            img.draw_string(2,2, res[0].payload(), color=(0,128,0), scale=2)
            #print(res[0].payload())
            m=res[0].payload()
        if m==str1:
            print('识别到12')
            KEY5.value(0)
            open1()
            close1()
            KEY5.value(1)
            qidong2()
            m=0
            t=0
        if m==str2:
            print('识别到21')
            KEY6.value(0)
            open2()
            close2()
            KEY6.value(1)
            qidong2()

            m=0
            t=0
        if m==str3:
            print('识别到11')
            KEY5.value(0)
            open1()
            close1()
            KEY5.value(1)
            qidong2()
            m=0
            t=0
        if m==str4:
            print('识别到22')
            KEY6.value(0)
            open2()
            close2()
            KEY6.value(1)
            qidong2()
            m=0
            t=0


