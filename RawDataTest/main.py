#Credit to Shilleh on Youtube for the communication part of this code
import machine
from machine import I2C, Pin
import time
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

while True:
    
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
    
    
    print(str(gx) + " " + str(gy) + " " + str(gz) + " " + str(ax) + " " + str(ay) + " " + str(az))
    time.sleep(0.001) #Could change this. Just pulled this out of thin air