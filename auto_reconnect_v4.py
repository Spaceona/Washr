from machine import Pin, I2C
from imu import MPU6050
from time import sleep
import network
import urequests
import requests
import json

# Shows Pi is on by turning on LED when plugged in
LED = Pin("LED", Pin.OUT)
LED.on()

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
        print("Offline. Attempting to reconnect...")
        try:
            reconnect_attempts = 0
            while not checkConnection() and reconnect_attempts < 10:
                wifi.connect(ssid, password)
                print("Trying to reconnect...")
                reconnect_attempts += 1
                sleep(30)
            if checkConnection():
                print("Reconnected to Wi-Fi")
                print("IP Address:", wifi.ifconfig()[0])
        except Exception as e:
            print("Failed to reconnect:", e)
            sleep(2)

while True:
    try:
        ax = round(imu.accel.x, 2)
        ay = round(imu.accel.y, 2)
        az = round(imu.accel.z, 2)
        gx = round(imu.gyro.x)
        gy = round(imu.gyro.y)
        gz = round(imu.gyro.z)
        tem = round(imu.temperature, 2)

        x_condition = check_threshold(gx)
        y_condition = check_threshold(gy)
        z_condition = check_threshold(gz)

        result = x_condition + y_condition + z_condition >= 1

        # Update counters
        if result:
            true_count += 1
        else:
            false_count += 1

        # Print the result and counters
        # print(f"ax {ax}\tay {ay}\taz {az}\tgx {gx}\tgy {gy}\tgz {gz}\tTemperature {tem}", end="\r")
        # print(f"Result: {result} True Count: {true_count} False Count: {false_count}")

        # Keep only the last 15 counts
        true_count %= 15
        false_count %= 15

        # Check and update final result every 15 iterations
        if (true_count + false_count) % 15 == 0:
            final_result = true_count > false_count
            print(f"Final Result after 15 iterations: {final_result}")

            # Send final_result over Wi-Fi
            try:
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

            true_count = 0
            false_count = 0

        sleep(0.2)

    except Exception as e:
        print("An error occurred:", e)
        sleep(2)  # Wait for 2 seconds before retrying
