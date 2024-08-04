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

extern Timezone myTimezone;
extern String timezone_name;

extern int led_1;
extern int led_2;

#endif //GLOBALS_H
