#ifndef WIFI_FUNCTION_H
#define WIFI_FUNCTION_H


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ezTime.h>
#include "wifi_secrets.h"
#include "globals.h"

//Initializes the wifi
void wifi_init(String server_name, HTTPClient& https);

void wifiConnect();

boolean serverAuth();

void machineStatusUpdate(boolean currentMachineStatus);

//Used to check a webserver for a new firmware version (WIP)
void ota_update(WiFiClient ota_client, String ota_server_url, uint16_t ota_port, String ota_firmware_location);

//Used to sync a clock to a NTP server to get the local time (WIP)
void setClock();

void firmwareCheck();

time_t firmwareUpdateTime();

#endif
