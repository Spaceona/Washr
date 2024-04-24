#Test reading the password from secrets.txt, and then starting up the ap mode
from imu import MPU6050
from machine import Pin, I2C
import network
import time
import socket
import calibration

i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
imu = MPU6050(i2c)

# Function to get accelerometer and gyroscope data
def get_imu_data_calibrated(x_offset, y_offset, z_offset):
    ax, ay, az = imu.accel.x, imu.accel.y, imu.accel.z
    gx, gy, gz = imu.gyro.x - x_offset, imu.gyro.y - y_offset, imu.gyro.z - z_offset
    return {
        'accelerometer': {'x': ax, 'y': ay, 'z': az},
        'gyroscope': {'x': gx, 'y': gy, 'z': gz}
    }

# Function to get accelerometer and gyroscope data
def get_imu_data():
    ax, ay, az = imu.accel.x, imu.accel.y, imu.accel.z
    gx, gy, gz = imu.gyro.x, imu.gyro.y, imu.gyro.z
    return {
        'accelerometer': {'x': ax, 'y': ay, 'z': az},
        'gyroscope': {'x': gx, 'y': gy, 'z': gz}
    }

with open("secrets.txt", encoding='utf8') as file_object:
    
    data = file_object.read().splitlines()
    ssid = data[0][5:]
    password = data[1][9:]
    api_key = data[2][8:]
    setup_complete = data[3][15:]

if(setup_complete == 'true' or setup_complete == 'True'):
    setup_complete = True
    #print("Setup complete was: " + str(setup_complete))
    #print("test")
            
    with open("offsets.txt", "r", encoding='utf8') as file_object:
        offsets = file_object.read().splitlines()
        print('Offsets read are %s ' % offsets)
        x_offset = float(offsets[0])
        y_offset = float(offsets[1])
        z_offset = float(offsets[2])
elif(setup_complete == 'false' or setup_complete == 'False'):
    setup_complete = False
    
    #Calibration Code
    # Wait for MPU to Settle
    settling_time = 4
    print('Settling MPU for %d seconds' % settling_time)
    time.sleep(4)
    print('MPU is Done Settling')
    
    calculated_offsets = calibration.gyro_calibration(100)
    print(calculated_offsets)
    #print(type(calculated_offsets))
    with open("offsets.txt", "w", encoding='utf8') as file_object:
        for index in calculated_offsets:
            file_object.write(str(index) + '\n')
            
    setup_complete = True
    with open("offsets.txt", "r", encoding='utf8') as file_object:
        offsets = file_object.read().splitlines()
        print('Offsets read are %s ' % offsets)
        x_offset = float(offsets[0])
        y_offset = float(offsets[1])
        z_offset = float(offsets[2])
    
else:
    pass

print(data)
print(ssid)
print(password)
print(api_key)
print(setup_complete)

"""
if(setup_complete == True):
    setup_complete = False
elif(setup_complete == False):
    setup_complete = True
    """
    
# Initialize data_out as a list with four elements
data_out = ["", "", "", ""]
data_out[0]= "SSID=" + str(ssid)
data_out[1] = "Password=" + str(password)
data_out[2] = "api_key=" + str(api_key)
data_out[3] = "setup_complete=" + str(setup_complete)

# Write the modified lines back to the file
with open("secrets.txt", "w", encoding='utf8') as file_object:
    for line in data_out:
        file_object.write(line + '\n')


#Reading from the gyro and printing the new values with the offset
while(True):
    #print(get_imu_data())
    print(get_imu_data_calibrated(x_offset, y_offset, z_offset))
    time.sleep(1)
