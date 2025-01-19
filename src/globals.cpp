//
// Created by leslier on 8/3/2024.
//

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ezTime.h>
#include <Preferences.h>
#include "flashStorage.h"
#include "globals.h"

//Firmware version
String FIRMWARE_VERSION = "1-6-4";
//TODO fix this for double digit values
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
//TODO change the client_key and client_name later to not be in the wifi_secrets.h file
String clientKey = CLIENT_KEY;
String clientName = CLIENT_NAME;

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
#ifdef SEEEED_XIAO_ESP32C3
int led_1 = D3;
int led_2 = D2;
#elif CUSTOM_BOARD
int led_1 = 6;
int led_2 = 10;
#endif

boolean machineStatus = false;
boolean previousMachineStatus = false;
//Confidence value for the machine detection
int detectionConfidence = 0;
boolean heartbeatSent = false;

//Setting up the flash storage
Preferences flashStorage;