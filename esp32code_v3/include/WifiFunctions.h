// Created by Robbie Leslie
// Modified by

#ifndef WIFI_FUNCTION_H
#define WIFI_FUNCTION_H


#include <Arduino.h>
#include <ezTime.h>



//Initializes the wifi
boolean wifi_init(String server_name, HTTPClient& https);

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
