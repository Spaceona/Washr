from machine import Pin, I2C
from imu import MPU6050
from time import sleep
import network
import urequests
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
ssid = "Guntas"
password = "qwertyuiop"

# Set the Wi-Fi mode to station (client)
wifi.active(True)
print("Connecting to Wi-Fi...")

while True:
    if not wifi.isconnected():
        print("Offline. Attempting to reconnect...")
        try:
            wifi.connect(ssid, password)
            reconnect_attempts = 0
            while not wifi.isconnected() and reconnect_attempts < 10:
                print("Trying to reconnect...")
                reconnect_attempts += 1
                sleep(1)
            if wifi.isconnected():
                print("Reconnected to Wi-Fi")
                print("IP Address:", wifi.ifconfig()[0])
        except Exception as e:
            print("Failed to reconnect:", e)
            sleep(2)
            continue

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
                final_result_url = 'https://spotr-22162-default-rtdb.firebaseio.com/final_result.json'
                headers = {'Content-Type': 'application/json'}
                data = json.dumps({"final_result": final_result})
                response = urequests.put(final_result_url, data=data, headers=headers)
                print(response.text)
                response.close()
            except Exception as e:
                print("Failed to send final_result:", e)

            true_count = 0
            false_count = 0

        sleep(0.2)

    except Exception as e:
        print("An error occurred:", e)
        sleep(2)  # Wait for 2 seconds before retrying
