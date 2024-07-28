#Test reading the password from secrets.txt, and then starting up the ap mode

import network
import time
import socket

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
elif(setup_complete == 'false' or setup_complete == 'False'):
    setup_complete = False
else:
    pass

print(data)
print(ssid)
print(password)
print(api_key)
print(setup_complete)

if(setup_complete == True):
    setup_complete = False
elif(setup_complete == False):
    setup_complete = True
    
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


#Reading from file works, now test the AP mode
