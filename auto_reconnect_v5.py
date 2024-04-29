#Autoreconnect v4 but using a state machine to handle logic

import micropython
import gc
from machine import Pin, I2C
from imu import MPU6050
from time import sleep
import network
import urequests
import utime
import requests
import json

# Shows Pi is on by turning on LED when plugged in
LED = Pin("LED", Pin.OUT)
LED.on()
pinValue = 0

#Setting up the colored LEDs
#Red connected to GPIO 20
red_led = Pin(20, Pin.OUT)
#Green connected to GPIO 21
green_led = Pin(21, Pin.OUT)
#Blue connected to GPIO 22
blue_led = Pin(22, Pin.OUT)
#Turning off the leds at the start of the program
red_led.value(0)
green_led.value(0)
blue_led.value(0)

i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
imu = MPU6050(i2c)

def check_threshold(value):
    return value > 2 or value < -1

# Initialize counters
true_count = 0
false_count = 0
final_result = False

# Initialize Wi-Fi interface
wifi = network.WLAN(network.STA_IF)

# Activate Wi-Fi
wifi.active(False)
wifi.active(True)

# Define the SSID and password of the network
ssid = "pards"
password = ""

# Set the Wi-Fi mode to station (client)
wifi.active(True)
print("Connecting to Wi-Fi...")

#declaring the functions
#checkConnection (ssid, password)
#attemptConnection (ssid, password) 

#Checks the status of the devices connection
def checkConnection():
    if wifi.isconnected():
        return True
    return False


# Function to get accelerometer and gyroscope data
def get_imu_data():
    ax, ay, az = imu.accel.x, imu.accel.y, imu.accel.z
    gx, gy, gz = imu.gyro.x, imu.gyro.y, imu.gyro.z
    return {
        'accelerometer': {'x': ax, 'y': ay, 'z': az},
        'gyroscope': {'x': gx, 'y': gy, 'z': gz}
    }
    
#used to reconnect to internet if device is disconnected
def attemptConnection (ssid, password):
    if not checkConnection():
        #Setting the blue and red leds on to indicate a wifi connection error
        red_led.value(1)
        green_led.value(0)
        blue_led.value(1)
        print("Offline. Attempting to reconnect...")
        try:
            while not checkConnection():
                wifi.active(False)
                wifi.active(True)
                wifi.connect(ssid, password)
                print("Trying to reconnect...")
                sleep(1) #Originally was 30 but I changed it
            if checkConnection():
                print("Reconnected to Wi-Fi")
                print("IP Address:", wifi.ifconfig()[0])
                 #setting the blue and green leds to show that it has reconnected
                red_led.value(0)
                green_led.value(1)
                blue_led.value(1)
        except Exception as e:
            #Setting the red and green leds to indicate a non-connection related error
            red_led.value(1)
            green_led.value(1)
            blue_led.value(0)
            print("Failed to reconnect:", e)
            #sleep(2)


#Setting up the state machine
state = 'default'
timer1 = utime.ticks_ms() #need to change this to utime to get miliseconds and then update the period
period = 3000 #period in miliseconds

num_ticks = 0


#Tick function used for the state machine
def tickFunction(state, pinValue, num_ticks):
    
    #print("Starting state: " + state) #Debugging statement
    
    
    #State machine code
    if(state == 'default'): #Default state. Used just in case
        #State Transitions
        state = 'transmit' #Alwasy go to transmit state first
        
        #setting just the green led to show that it is in the default state
        red_led.value(0)
        green_led.value(1)
        blue_led.value(0)
        
        #State logic would go here if there was any    
        
        #Returning the "global" variables"
        return state, pinValue, num_ticks
        
    elif(state == 'transmit'):
        #State Transitions
        state = 'transmit' #repeated transmission
        
        #Running the garbage collector every 20 cycles to make sure it doesn't run out of memory
        num_ticks += 1
        print(num_ticks) #Debugging statement
        if(num_ticks == 20):
            micropython.mem_info() #printing the memory of the pico to make sure it is good
            gc.collect()
            micropython.mem_info() #printing the memory of the pico to make sure it is good
            num_ticks = 0 #resetting the number of ticks
            
        
        
        
        #State logic
        # Send final_result over Wi-Fi
        try:
            
            #Making sure that the board is connected
            if not checkConnection():
                attemptConnection(ssid, password)
            
             #Blinking the LED to show that it is transmitting
            if(pinValue == 0):
                LED.on()
                pinValue = 1
            else:  
                LED.off()
                pinValue = 0
            
            url = 'https://api.spaceona.com/update/lafayette.edu/watsonhall/washer/0/'+(str(final_result)).lower()+'?token=NpLvwbWzkgrpq2UZem9TbfN4s6gcBTiNuaoqA3Ap9S9csrEp'
            headers = {'Content-Type': 'application/json'}
            final_result_post = urequests.post(url, data=json.dumps(get_imu_data()), headers=headers)
            print(final_result_post.status_code)
            print(final_result_post.text)
            #Setting just the blue led to show that it is transmitting correctly
            red_led.value(0)
            green_led.value(0)
            blue_led.value(1)
        except Exception as e:
            if not checkConnection():
                attemptConnection(ssid, password)
                
            else:
                #Setting the red and green leds to indicate a non-connection related error
                red_led.value(1)
                green_led.value(1)
                blue_led.value(0)
                print("Error: unrelated to connection", e)
                
        #Returning the "global" variables"
        return state, pinValue, num_ticks
    
    else:
        #Setting the state machine to the default state if something goes wrong
        state = 'default'
        
        #Returning the "global" variables"
        return state, pinValue, num_ticks
    #print("Next State: " + state) #Debugging statement

while True:
    #Calling the timer used to check period
    timer2 = utime.ticks_ms()
    
    #Checking if the period has elapsed
    if((timer2 - timer1) >= period):
        #Calling the tick function
        state, pinValue, num_ticks = tickFunction(state, pinValue, num_ticks)
        timer1 = timer2
