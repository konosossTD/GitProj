from machine import Timer,PWM
import time
#PWM 通过定时器配置，接到 IO17 引脚
tim = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PWM)
tim1 = Timer(Timer.TIMER1, Timer.CHANNEL1, mode=Timer.MODE_PWM)

S1 = PWM(tim, freq=50, duty=0, pin=17)
S2 = PWM(tim1, freq=50, duty=0, pin=35)
'''
说明：舵机控制函数
功能：180 度舵机：angle:0 至 180 表示相应的角度
 360 连续旋转度舵机：angle:-90 至 90 旋转方向和速度值。
 【duty】占空比值：0-100
'''
a=0
b=0
def Servojia1(servo,angle):
    global  a
    while(a<angle):
        S1.duty((a)/270*10+2.5)
        a += 1
        time.sleep(0.005)
        print("a=",a)

def Servojian1(servo,angle):
    global  a
    while(a>angle):
        S1.duty((a)/270*10+2.5)
        a -= 1
        time.sleep(0.005)
        print("a=",a)

def Servojia2(servo,angle):
    global  b
    while(b<angle):
        S2.duty((b)/270*10+2.5)
        b += 1
        time.sleep(0.005)
        print("b=",b)

def Servojian2(servo,angle):
    global  b
    while(b>angle):
        S2.duty((b)/270*10+2.5)
        b -= 1
        time.sleep(0.005)
        print("b=",b)


def open1():
    Servojia1(S1,0)
    time.sleep(1)
    Servojia1(S1,110)
    time.sleep(2)

def close1():
    Servojian1(S1,110)
    time.sleep(3)
    Servojian1(S1,0)
    time.sleep(3)

def open2():
    Servojia2(S2,0)
    time.sleep(1)
    Servojia2(S2,90)
    time.sleep(3)

def close2():
    Servojian2(S2,90)
    time.sleep(3)
    Servojian2(S2,0)
    time.sleep(2)


