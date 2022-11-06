'''
实验名称：颜色识别
版本： v1.0
日期： 2019.12
作者： 01Studio
实验目的：单个颜色识别
'''
import sensor,lcd,time
from anjian import  qidong2

#摄像头初始化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_vflip(1) #后置模式，所见即所得

#lcd初始化
lcd.init()

clock=time.clock()

# 颜色识别阈值 (L Min, L Max, A Min, A Max, B Min, B Max) LAB模型
# 下面的阈值元组是用来识别 红、绿、蓝三种颜色，当然你也可以调整让识别变得更好。
thresholds = [(55, 100, 18, 127, -8, 127), # 红色阈值
              (76, 100, -128, -25, -16, 127), # 绿色阈值
              (0, 30, 0, 64, -128, -20)] # 蓝色阈值

#f=1

def color():
    global blob
    while True:
        #clock.tick()
        img=sensor.snapshot()
        lcd.display(img)     #LCD显示图片
        #left_roi = [50,30,60,60]  #设置感兴趣区域
        #for blob in img.find_blobs(thresholds,pixels_threshold=1000,merge = True): # 0,1,2分别表示红，绿，蓝色。
            #if blob.code() == 1:
                ##tmp=img.draw_rectangle(b[0:4])
                #tmp=img.draw_circle(blob[5],blob[6],30)
                #tmp=img.draw_cross(blob[5], blob[6])
                #print("识别到红色")
        for blob in img.find_blobs(thresholds,pixels_threshold=1000,merge = True): # 0,1,2分别表示红，绿，蓝色。
            if blob.code() == 2:
                #tmp=img.draw_rectangle(b[0:4])
                tmp=img.draw_circle(blob[5],blob[6],30)
                tmp=img.draw_cross(blob[5], blob[6])
                print("识别到绿色")
                #f=0
                qidong2()
        print(clock.fps())   #打印FPS
