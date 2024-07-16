#ifndef TICK_FUNCTION_H
#define TICK_FUNCTION_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <NetworkClientSecure.h>
#include <HTTPClient.h>

void tickFunction(Adafruit_MPU6050 &mpu, const char* server_name, NetworkClientSecure &client, HTTPClient &https, int &led_1);

#endif