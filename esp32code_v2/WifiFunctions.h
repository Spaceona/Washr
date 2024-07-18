#ifndef WIFI_FUNCTION_H
#define WIFI_FUNCTION_H


#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "wifi_secrets.h"

void wifi_init(const char* server_name, NetworkClientSecure &client, HTTPClient &https);
void ota_update(WiFiClient ota_client, String ota_server_url, uint16_t ota_port, String ota_firmware_location);

#endif