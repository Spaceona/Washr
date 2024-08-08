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
#include "wifi_secrets.h"

extern String FIRMWARE_VERSION;
extern int FIRMWARE_VERSION_MAJOR;
extern int FIRMWARE_VERSION_MINOR;
extern int FIRMWARE_VERSION_PATCH ;

extern Timezone myTimezone;
extern String timezone_name;

extern WiFiClientSecure client;

extern String clientKey;

extern String server_name;
extern String endpoint;

extern String clientName;
extern String building;
extern String type;
extern String id;

extern String jwt;
extern boolean authenticated;

extern int led_1;
extern int led_2;

extern const char* ssid;
extern const char* password;
extern String mac_address;

extern int detectionConfidence;

#endif //GLOBALS_H
