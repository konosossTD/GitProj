from machine import UART,Timer
from Maix import GPIO
from fpioa_manager import fm
#映射串口引脚
fm.register(6, fm.fpioa.UART1_RX, force=True)
fm.register(7, fm.fpioa.UART1_TX, force=True)
fm.register(16, fm.fpioa.GPIO1)
#初始化串口
uart = UART(UART.UART1, 9600, read_buf_len=4096)
KEY = GPIO(GPIO.GPIO1, GPIO.IN)
#KEY6 = GPIO(GPIO.GPIO6, GPIO.IN)
#uart.write('Hello 01Studio!')


def qidong():
    data = bytearray([0x4B])
    data1 = bytearray([0x5A])
    if KEY.value()==0:
        uart.write(data1)
        uart.write(data)
    #if KEY6.value()==0:
        #uart.write(data1)

def qidong2():
    data2 = bytearray([0x4D])
    uart.write(data2)

#def  jieshou():
    #global c
    #v=0
    #while(v==0):
        #if uart.any():
            #c=uart.readline().decode()
            #print(c)

        #c = uart.read()   #读入数据
        #print(c)
        #if c!=None:
            ##tmp_data=int(c)     #强制转换为整型变量
            #print(tmp_data)
        #if c==1:
            #print('qqq')
            #v=1
        #if c==2:
            #v=2
    #return v

#while True:
    #if uart.any():
        #a=uart.readline().decode()
        #print(a)

