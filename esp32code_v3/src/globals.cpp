//
// Created by leslier on 8/3/2024.
//

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ezTime.h>
#include "globals.h"

//Setting up the timezone for ezTime
Timezone myTimezone;
String timezone_name = "America/New_York";

//Global wifi client
WiFiClientSecure client;

//TODO change this to the backend server when backend changes are merged
String server_name = "http://172.31.2.17:3000";
String endpoint = "";

//A secret key which is different for each SpaceOna client (used for authentication)
//We might want an initial key so the board can connect to the backend and then get the correct client key
String clientKey = "test";

String jwt = "";
boolean authenticated = false;

// Setting up the wifi details
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
String mac_address;

// Setting up the LEDs
int led_1 = D3;
int led_2 = D2;