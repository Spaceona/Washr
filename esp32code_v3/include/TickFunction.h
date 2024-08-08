#ifndef TICK_FUNCTION_H
#define TICK_FUNCTION_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "globals.h"

//Tick function for the central logic. Right now just constantly transmits the raw sensor data
void tickFunction(Adafruit_MPU6050 &mpu, const char* server_name, HTTPClient &https);

#endif