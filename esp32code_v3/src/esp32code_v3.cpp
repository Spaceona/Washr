// SpaceOna Washr ESP32 code V1

// Imports
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ezTime.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "MPUFunctions.h"
#include "TickFunction.h"
#include "WifiFunctions.h"
#include "flashStorage.h"





// Setting up the MPU
Adafruit_MPU6050 mpu;



void setup() {
    // Setting up the Serial
    Serial.begin(115200);
    delay(1000);

    // Setting up the Serial1 for debugging
    //Serial1.begin(115200, SERIAL_8N1, D7, D6);

    // Setting up the LED
    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);

    //This is just used to allow the user to have time to connect to serial
    delay(5000);

    flashStorageInit();

    //Initializing MPU
    mpu_init(mpu, MPU6050_RANGE_2_G, MPU6050_RANGE_500_DEG, MPU6050_BAND_5_HZ);

    //setting the wifi credentials for testing
    setWifiCredentials(WIFI_SSID, WIFI_PASSWORD);

}

// Setting up the timers for the tick function state machine
unsigned long tick_timer1 = 0;
unsigned long tick_timer2 = millis();
int tick_period = 15000; //In milliseconds

// Setting up the timers for the mpu debounce state machine
unsigned long mpu_timer1 = 0;
unsigned long mpu_timer2 = millis();
int mpu_period = 100; //In milliseconds

void loop() {
    // Used for ezTime events
    events();

    //Updating the tick functions timers
    tick_timer2 = millis();
    mpu_timer2 = millis();

    // Updating the MPUs debouncing state machine
    if ((mpu_timer2 - mpu_timer1) >= mpu_period) {
        machineStatus = mpu_tick(mpu);
        //Serial.println("Machine status: ");
        //Serial.println(in_use);
        mpu_timer1 = mpu_timer2;
    }

    //If the server takes a long time to respond, it won't call the mpu function so it doesn't update the status correctly. The ESP32C3 doesn't have multithreading, so need to fix that
    if ((tick_timer2 - tick_timer1) >= tick_period) {
        //Serial.println("Tick function reached");
        tickFunction();
        tick_timer1 = tick_timer2;
    }
}
