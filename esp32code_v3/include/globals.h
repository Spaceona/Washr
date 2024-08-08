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

#define FIRMWARE_VERSION "1-0-0";

extern Timezone myTimezone;
extern String timezone_name;

extern WiFiClientSecure client;

extern String clientKey;

extern String server_name;
extern String endpoint;

extern String jwt;
extern boolean authenticated;

extern int led_1;
extern int led_2;

extern const char* ssid;
extern const char* password;
extern String mac_address;

#endif //GLOBALS_H
