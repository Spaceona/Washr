#Credit to Shilleh on Youtube for the communication part of this code
import machine
from machine import I2C, Pin
import time
#from machine import Pin
import uos

led = Pin("LED", Pin.OUT)
pinValue = 0

# need this UART to read from BME and be able to send data to local computer
uart = machine.UART(0, baudrate=115200)
uart.init(115200, bits=8, parity=None, stop=1, tx=Pin(0), rx=Pin(1))
uos.dupterm(uart)

testVariable = 1

while True:
    
    if(pinValue == 0):
        led.on()
        pinValue = 1
    else:  
        led.off()
        pinValue = 0
    
    print(str(testVariable))
    time.sleep(0.3) #Could change this. Just pulled this out of thin air
    testVariable += 1