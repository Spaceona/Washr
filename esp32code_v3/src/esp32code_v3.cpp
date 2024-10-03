// SpaceOna Washr ESP32 code V1

// Imports
#include <Arduino.h>

#ifdef SEEEED_XIAO_ESP32C3
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MPUFunctions.h"
#elif CUSTOM_BOARD
#include <ICM42670P.h>
#include "IMUFunctions.h"
#endif

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ezTime.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "TickFunction.h"
#include "WifiFunctions.h"
#include "flashStorage.h"


#ifdef SEEEED_XIAO_ESP32C3
// Setting up the MPU
Adafruit_MPU6050 mpu;
#elif CUSTOM_BOARD
ICM42670 imu(Wire,0);
int accelRange = 2;
int gyroRange = 2000;
#endif


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
    delay(3000);

    flashStorageInit();

    #ifdef SEEEED_XIAO_ESP32C3
    //Initializing MPU6050 on seeed board
    mpu_init(mpu, MPU6050_RANGE_2_G, MPU6050_RANGE_500_DEG, MPU6050_BAND_5_HZ);
    #elif CUSTOM_BOARD
    //Initializing the IMU on the ESP32C3
    imu_init(imu, accelRange, gyroRange);

    #endif
    //setting the wifi credentials for testing
    //setWifiCredentials(WIFI_SSID, WIFI_PASSWORD);

}

// Setting up the timers for the tick function state machine
unsigned long tick_timer1 = 0;
unsigned long tick_timer2 = millis();
int tick_period = 5000; //In milliseconds
//TODO set up a way to change the period of this state machine dynamically from the backend



#ifdef SEEEED_XIAO_ESP32C3
int mpu_period = 100; //In milliseconds
// Setting up the timers for the mpu debounce state machine
unsigned long mpu_timer1 = 0;
unsigned long mpu_timer2 = millis();
#elif CUSTOM_BOARD
int imu_period = 100; //In milliseconds
// Setting up the timers for the imu debounce state machine
unsigned long imu_timer1 = 0;
unsigned long imu_timer2 = millis();
#endif


void loop() {
    // Used for ezTime events
    events();

    //Updating the tick functions timers
    tick_timer2 = millis();

    #ifdef SEEEED_XIAO_ESP32C3
    mpu_timer2 = millis();
    #elif CUSTOM_BOARD
    imu_timer2 = millis();
    #endif

    #ifdef SEEEED_XIAO_ESP32C3
    // Updating the MPUs debouncing state machine
    if ((mpu_timer2 - mpu_timer1) >= mpu_period) {
        machineStatus = mpu_tick(mpu);
        //Serial.println("Machine status: ");
        //Serial.println(in_use);
        mpu_timer1 = mpu_timer2;
    }
    #elif CUSTOM_BOARD
    // Updating the IMUs debouncing state machine
    if ((imu_timer2 - imu_timer1) >= imu_period) {
        machineStatus = imu_tick(imu, accelRange, gyroRange);
        //Serial.println("Machine status: ");
        //Serial.println(machineStatus);
        imu_timer1 = imu_timer2;
    }
    #endif

    //If the server takes a long time to respond, it won't call the mpu function so it doesn't update the status correctly. The ESP32C3 doesn't have multithreading, so need to fix that
    if ((tick_timer2 - tick_timer1) >= tick_period) {
        //Serial.println("Tick function reached");
        tickFunction();
        tick_timer1 = tick_timer2;
    }
}
