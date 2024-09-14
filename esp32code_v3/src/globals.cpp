//
// Created by leslier on 8/3/2024.
//

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ezTime.h>
#include <preferences.h>
#include "flashStorage.h"
#include "globals.h"

//Firmware version
String FIRMWARE_VERSION = "1-3-0";
const int FIRMWARE_VERSION_MAJOR = FIRMWARE_VERSION.substring(0, 1).toInt();
const int FIRMWARE_VERSION_MINOR = FIRMWARE_VERSION.substring(2, 3).toInt();
const int FIRMWARE_VERSION_PATCH = FIRMWARE_VERSION.substring(4, 5).toInt();

//Setting up the timezone for ezTime
Timezone myTimezone;
String timezone_name = "America/New_York";

//Global wifi client
WiFiClientSecure client;

//TODO change this to the backend server when backend changes are merged
String server_name = "https://api.spaceona.com";
String endpoint = "";
String serverPort = "3001";


//A secret key which is different for each SpaceOna client (used for authentication)
//We might want an initial key so the board can connect to the backend and then get the correct client key
String clientKey = "_rQ8te)YMMMIqLGd7NaV7W_D^";
String clientName = "test3";

boolean setupComplete = false;

String jwt = "";
boolean authenticated = false;

// Setting up the wifi details
String ssid = "";
String password = "";
String mac_address;

//Setting up the parameterized versions of timing variables
int heartbeatPeriod = 1;

// Setting up the LEDs
int led_1 = D3;
int led_2 = D2;

boolean machineStatus = false;
//Confidence value for the machine detection
int detectionConfidence = 0;
boolean heartbeatSent = false;

//Setting up the flash storage
Preferences flashStorage;