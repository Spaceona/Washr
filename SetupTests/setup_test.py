#Test reading the password from secrets.txt, and then starting up the ap mode

import network
import time
import socket

with open("secrets.txt", encoding='utf8') as file_object:
    
    data = file_object.read().splitlines()
    ssid = data[0][5:]
    password = data[1][9:]
    api_key = data[2][8:]
    
print(data)
print(ssid)
print(password)
print(api_key)

#Reading from file works, now test the AP mode

def web_page():
  html = """<html><head><meta name="viewport" content="width=device-width, initial-scale=1"></head>
            <body><h1>Hello World</h1></body></html>
         """
  return html

def ap_mode(ssid, password):
    
    ap = network.WLAN(network.AP_IF)
    ap.config(essid=ssid, password=password)
    ap.active(True)
    
    while ap.active() == False:
        pass
    print('Ap is active, you can now connect')
    print('IP address to connect to:: ' + ap.ifconfig()[0])
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   #creating socket object
    s.bind(('', 80))
    s.listen(5)
    
    while True:
      conn, addr = s.accept()
      print('Got a connection from %s' % str(addr))
      request = conn.recv(1024)
      print('Content = %s' % str(request))
      response = web_page()
      conn.send(response)
      conn.close()
    s.close()
      
ap_mode('PicoTestAP', 'password')