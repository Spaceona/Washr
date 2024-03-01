#Credit to Shilleh on Youtube for the communication part of this code
import machine
from machine import I2C, Pin
import time
import utime
import uos

led = Pin("LED", Pin.OUT)
pinValue = 0

from imu import MPU6050

# need this UART to read from BME and be able to send data to local computer
uart = machine.UART(0, baudrate=115200)
uart.init(115200, bits=8, parity=None, stop=1, tx=Pin(0), rx=Pin(1))
uos.dupterm(uart)
i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
imu = MPU6050(i2c)

state = 'default'
timer1 = utime.ticks_ms() #need to change this to utime to get miliseconds and then update the period
period = 10 #period in miliseconds



def tickFunction():
    global state
    global pinValue
    global ax
    global ay
    global az
    global gx
    global gy
    global gz
    if(state == 'default'):
            state = 'sensor'
    elif(state == 'sensor'):
        state = 'transmit'
        
        if(pinValue == 0):
            led.on()
            pinValue = 1
        else:  
            led.off()
            pinValue = 0
    
        ax = round(imu.accel.x, 2)
        ay = round(imu.accel.y, 2)
        az = round(imu.accel.z, 2)
        gx = round(imu.gyro.x)
        gy = round(imu.gyro.y)
        gz = round(imu.gyro.z)
        
    elif(state == 'transmit'):
        state = 'sensor'
        
        print(str(gx) + " " + str(gy) + " " + str(gz) + " " + str(ax) + " " + str(ay) + " " + str(az))
    else:
        state = 'default'

while True:
    timer2 = utime.ticks_ms()
    
    #print(timer1)
    #print(timer2)
    #print(timer2 - timer1)
    
    if((timer2 - timer1) >= period):
        tickFunction()
        timer1 = timer2
    
