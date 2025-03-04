//
// Contains any global variables to be accessed by multiple files
//

#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ezTime.h>
#include <Preferences.h>
#include "wifi_secrets.h"

extern String FIRMWARE_VERSION;
extern const int FIRMWARE_VERSION_MAJOR;
extern const int FIRMWARE_VERSION_MINOR;
extern const int FIRMWARE_VERSION_PATCH ;

extern Timezone myTimezone;
extern String timezone_name;

extern WiFiClientSecure client;

extern String clientKey;
extern String clientName;

extern String server_name;
extern String endpoint;
extern String serverPort;

extern boolean setupComplete;

extern String jwt;
extern boolean authenticated;

extern int led_1;
extern int led_2;

extern String ssid;
extern String password;
extern String mac_address;

extern int heartbeatPeriod;

extern boolean machineStatus;
extern boolean previousMachineStatus;
extern int detectionConfidence;
extern boolean heartbeatSent;

extern Preferences flashStorage;

#endif //GLOBALS_H
