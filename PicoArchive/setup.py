#Autoreconnect v4 but using a state machine to handle logic

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

i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
imu = MPU6050(i2c)

def check_threshold(value):
    return value > 2 or value < -1

# Initialize counters
true_count = 0
false_count = 0
final_result = False



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

def web_page():
    #Need to get the html to host the webserver
    
def ap_mode(ssid, password):
    
    ap = network.WLAN(network.AP_IF)
    ap.config(essid=ssid, password=password)
    ap.active(True)
    
    while ap.active() == False:
        pass
    print('Ap is active, you can now connect')
    print('IP address to connect to:: ' ap.ifconfig()[0])
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   #creating socket object
    s.bind(('', 80))
    s.listen(5)
    
def ap_disconnect():
    #Figure out if this is necessary
    
def wifi_connect_mode():
    # Initialize Wi-Fi interface
    wifi = network.WLAN(network.STA_IF)

    # Activate Wi-Fi
    wifi.active(True)

    # Define the SSID and password of the network
    ssid = "pards"
    password = ""

    # Set the Wi-Fi mode to station (client)
    wifi.active(True)
    print("Connecting to Wi-Fi...")
    
    
#used to reconnect to internet if device is disconnected
def attemptConnection (ssid, password):
    if not checkConnection():
        print("Offline. Attempting to reconnect...")
        try:
            reconnect_attempts = 0
            while not checkConnection() and reconnect_attempts < 10:
                wifi.connect(ssid, password)
                print("Trying to reconnect...")
                reconnect_attempts += 1
                sleep(1) #Originally was 30 but I changed it
            if checkConnection():
                print("Reconnected to Wi-Fi")
                print("IP Address:", wifi.ifconfig()[0])
        except Exception as e:
            print("Failed to reconnect:", e)
            #sleep(2)


#Setting up the state machine
state = 'default'
timer1 = utime.ticks_ms() #need to change this to utime to get miliseconds and then update the period
period = 400 #period in miliseconds

#Tick function used for the state machine
def tickFunction():
    #Setting up global variables
    global state
    global pinValue
    #print("Starting state: " + state) #Debugging statement
    
    #State machine code
    if(state == 'default'): #Default state. Used just in case
        #State Transitions
        state = 'transmit' #Alwasy go to sensor state first

        #State logic would go here if there was any    

    elif(state == 'transmit'):
        #State Transitions
        state = 'transmit' #repeated transmission

        #State logic
        # Send final_result over Wi-Fi
        try:
            
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
        except Exception as e:
            if not checkConnection():
                attemptConnection(ssid, password)
            else:
                print("Error: unrelated to connection", e)
    else:
        #Setting the state machine to the default state if something goes wrong
        state = 'default'
    #print("Next State: " + state) #Debugging statement

while True:
    #Calling the timer used to check period
    timer2 = utime.ticks_ms()
    
    #Checking if the period has elapsed
    if((timer2 - timer1) >= period):
        #Calling the tick function
        tickFunction()
        timer1 = timer2
