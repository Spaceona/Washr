#ifndef WIFI_FUNCTION_H
#define WIFI_FUNCTION_H


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ArduinoJson.h>
#include <ezTime.h>
#include "globals.h"
#include "wifi_secrets.h"


//Initializes the wifi
void wifi_init(String server_name, HTTPClient& https);

void wifiConnect();

int serverAuth();

int machineStatusUpdate(boolean currentMachineStatus);

//Used to check a webserver for a new firmware version (WIP)
void otaUpdate(String updateFirmware);

int onboardBoard();

void requestCallback(HTTPClient* callbackClient);

//Used to sync a clock to a NTP server to get the local time (WIP)
void setClock();

void firmwareCheck();

time_t firmwareUpdateTime();

String latestFirmware();

time_t heartbeatUpdateTime(int minutePeriod);

void sendHeartbeat();

#endif
