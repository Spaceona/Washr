#ifndef WIFI_FUNCTION_H
#define WIFI_FUNCTION_H


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ezTime.h>
#include "wifi_secrets.h"

//Initializes the wifi
void wifi_init(const char* server_name, WiFiClientSecure &client, HTTPClient &https);

//Used to check a webserver for a new firmware version (WIP)
void ota_update(WiFiClient ota_client, String ota_server_url, uint16_t ota_port, String ota_firmware_location);

//Used to sync a clock to a NTP server to get the local time (WIP)
void setClock(Timezone &time_zone, String timezone_name);

#endif