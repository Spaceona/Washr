#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "globals.h"
#include "TickFunction.h"
#include "WifiFunctions.h"
#include "MPUFunctions.h"
#include "flashStorage.h"


// Setting up the https client for making HTTPS requests
HTTPClient https;

// Setting the states for the tick function
enum States {
    Start,
    getCreds,
    wifiInit,
    Transmit,
    Authenticate,
    Wait
} Sensor_State;

// Setting up the tick function
void tickFunction() {
    // Transitions
    //Serial.println("Starting State: " + String(Sensor_State)); //Debugging statement
    switch (Sensor_State) {
        case Start:
            if(WiFi.status() != WL_CONNECTED){
                Sensor_State = getCreds;
            } else if(!authenticated){
                Sensor_State = Authenticate;
                //Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                //Serial.println("Next state: Transmit");
            }
            break;
        case getCreds:
            if(hasWifiCredentials()){
                Sensor_State = wifiInit;
            } else {
                Sensor_State = getCreds;
            }
            break;
        case wifiInit:
            if(WiFi.status() == WL_CONNECTED){
                if(!authenticated){
                    Sensor_State = Authenticate;
                    //Serial.println("Next state: Authenticate");
                } else {
                    Sensor_State = Transmit;
                    //Serial.println("Next state: Transmit");
                }
            } else {
                Sensor_State = wifiInit;
                //In the future we might want more complex logic here to handle incorrect credentials and other problems
            break;
        }
        case Transmit:

            if(WiFi.status() != WL_CONNECTED){
                Sensor_State = getCreds;
            }

            if(!authenticated){
                Sensor_State = Authenticate;
                //Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                //Serial.println("Next state: Transmit");
            }
            break;
        case Authenticate:

            if(WiFi.status() != WL_CONNECTED){
                Sensor_State = getCreds;
            }

            if (!authenticated) {
                Sensor_State = Authenticate;
                //Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                //Serial.println("Next state: Transmit");
            }
            break;
        case Wait: //TODO Will be used for waiting for authentication
            Sensor_State = Transmit;
            break;
        default:
            Sensor_State = Start;
            break;
    }

    //Serial.println("Next State: " + String(Sensor_State)); //Debugging statement

    // State logic
    switch (Sensor_State) {
        case Start:
            // State logic would go here if there was any
            break;
        case getCreds:
            //Long term this is going to connect to bluetooth to get the credentials
            //Serial.println("Getting Credentials");
            ssid = getWifiSsid();
            password = getWifiPassword();
            break;
        case wifiInit: {
            //Initializing the Wifi
            wifi_init(server_name, https);

            //Waiting for the clock to sync
            waitForSync();

            //Setting up the time to check for firmware updates (also sets up the clock)
            time_t firmwareTime = firmwareUpdateTime();

            //Setting up the firmware check event
            myTimezone.setEvent(firmwareCheck, firmwareTime);

            //Authenticate with the server
            //serverAuth();
            break;
        }
        case Transmit:
            //Serial.println("Transmitting");
            digitalWrite(led_1, LOW);
            // Checking to make sure the wifi is configured
            if (WiFi.status() != WL_CONNECTED) {
                wifiConnect();
            } else if(!authenticated) {
                break;
            } else {
                // Transmitting the data
                machineStatusUpdate(machineStatus);
            }
            break;
        case Authenticate:
            //Serial.println("Authenticating");
            digitalWrite(led_1, LOW);
            if(WiFi.status() != WL_CONNECTED){
                wifiConnect();
            }
            authenticated = serverAuth();
            break;
        default:
            break;
    }
}