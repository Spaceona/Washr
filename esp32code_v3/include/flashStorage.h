//
// Created by leslier on 8/17/2024.
//

#ifndef ESP32CODE_V3_FLASHSTORAGE_H
#define ESP32CODE_V3_FLASHSTORAGE_H

#include <Arduino.h>
#include <preferences.h>
#include <ArduinoJson.h>
#include "globals.h"

boolean flashStorageInit();

boolean hasWifiCredentials();

boolean setHasWifiCredentials(boolean hasCredentials);

boolean setWifiCredentials(String newSsid, String newPassword);

String getWifiSsid();

String getWifiPassword();

void clearWifiCredentials();



#endif //ESP32CODE_V3_FLASHSTORAGE_H
