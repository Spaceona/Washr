#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClientSecure.h>
#include <HTTPClient.h>
#include "wifi_secrets.h"

void wifi_init(const char* server_name, NetworkClientSecure &client, HTTPClient &https);