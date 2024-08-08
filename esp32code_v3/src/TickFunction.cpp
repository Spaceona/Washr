#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "TickFunction.h"
#include "WifiFunctions.h"
#include "MPUFunctions.h"
#include "globals.h"


// Setting the states for the tick function
enum States {
    Start,
    Transmit,
    Authenticate,
    Wait
} Sensor_State;

// Setting up the tick function
void tickFunction() {
    Serial.println("Auth: " + String(authenticated));
    // Transitions
    switch (Sensor_State) {
        case Start:
            if(!authenticated){
                Sensor_State = Authenticate;
                Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                Serial.println("Next state: Transmit");
            }
            break;
        case Transmit:
            if(!authenticated){
                Sensor_State = Authenticate;
                Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                Serial.println("Next state: Transmit");
            }
            break;
        case Authenticate:
            if (!authenticated) {
                Sensor_State = Authenticate;
                Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                Serial.println("Next state: Transmit");
            }
            break;
        case Wait: //TODO Will be used for waiting for authentication
            Sensor_State = Transmit;
            break;
        default:
            Sensor_State = Start;
            break;
    }

    // State logic
    switch (Sensor_State) {
        case Start:
            // State logic would go here if there was any
            break;
        case Transmit:
            Serial.println("Transmitting");
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
            Serial.println("Authenticating");
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