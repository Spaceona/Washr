#Credit to Shilleh on Youtube for the code
import serial
import time

start_time = time.time()

# Configure the serial connection
port = "/dev/cu.usbmodem11101" #Change this to your serial connection. Will be OS dependent
baudrate = 115200
serial_connection = serial.Serial(port, baudrate)

# Open a file on your computer to write the received data
destination_file = open("rawdata.txt", "w")
data = 1
stopProgram = 1

destination_file.write("Time GyroX GyroY GyroZ AccelX AccelY AccelZ \n")

# Read and write data until the transfer is complete
while True:
    
    
    if(stopProgram > 20):
        break
    
    data = serial_connection.readline().decode('utf-8').rstrip()
    if data == b"EOF":
        break
    elapsed_time = time.time() - start_time
    print(str(elapsed_time) + data)
    destination_file.write(str(elapsed_time) + " " + str(data) + "\n")
    #data += 1
    #stopProgram += 1
    if(elapsed_time > 1980):
        print("Finishing")
        break

# Close the files and serial connection
destination_file.close()
serial_connection.close()