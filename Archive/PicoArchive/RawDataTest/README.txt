The main.py is the code which should be uploaded to the Pico.
connect_to_pico.py is the code which will be ran locally to connect to the Pico and get the raw data in a text file.
In order to run connect_to_pico.py, you must install the pyserial library using "pip3 install pyserial" 
You also should not have an IDE like Thonny or VSCode which is connected to the pico while running connect_to_pico.py.
You will need to find the serial connection to the Pico and modify connect_to_pico.py to reference that serial connection.