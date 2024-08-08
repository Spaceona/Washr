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
void tickFunction(Adafruit_MPU6050 &mpu, HTTPClient &https) {

    // Transitions
    switch (Sensor_State) {
        case Start:
            Sensor_State = Transmit;
            break;
        case Transmit:
            if(authenticated){
                Sensor_State = Transmit;
            } else {
                Sensor_State = Authenticate;
            }
            break;
        case Authenticate:
            if (authenticated) {
                Sensor_State = Transmit;
            } else {
                Sensor_State = Authenticate;
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

            digitalWrite(led_1, LOW);

            //Serial.println("Entered Transmit stage!");
            // Checking to make sure the wifi is configured
            if (WiFi.status() != WL_CONNECTED) {
                wifiConnect();
            } else {



                // Transmitting the data
                Serial.println("Starting HTTPS connection...");
                if (!https.begin(client, server_name)) {
                    Serial.println("Failed to start HTTPS connection");
                    return;
                }
                https.addHeader("Content-Type", "application/json");
                String httpsPostData = get_mpu_data(mpu);
                Serial.println("Sending POST request...");
                int httpsResponseCode = https.POST(httpsPostData);
                Serial.print("HTTPS Response code: ");
                Serial.println(httpsResponseCode);
                //Should probably add better error code handling here
                if (httpsResponseCode <= 0) {
                    Serial.println("Failed to send POST request");
                    return;
                } else {
                    digitalWrite(led_1, HIGH);
                    delay(100);
                }
                Serial.println("");
                digitalWrite(led_1, LOW);

            }
            break;
        case Authenticate:
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