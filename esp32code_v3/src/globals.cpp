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

// Setting up the LEDs
int led_1 = D3;
int led_2 = D2;