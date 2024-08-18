//
// Created by leslier on 8/17/2024.
//

#ifndef ESP32CODE_V3_FLASHSTORAGE_H
#define ESP32CODE_V3_FLASHSTORAGE_H

#include <Arduino.h>
#include <preferences.h>
#include <ArduinoJson.h>
#include "globals.h"

void flashStorageInit();

boolean hasWifiCredentials();

void setHasWifiCredentials(boolean hasCredentials);

void setWifiCredentials(String newSsid, String newPassword);

String getWifiSsid();

String getWifiPassword();

void clearWifiCredentials();

void flashStorageClose();

#endif //ESP32CODE_V3_FLASHSTORAGE_H
