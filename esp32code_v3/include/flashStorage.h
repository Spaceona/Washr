//
// Created by leslier on 8/17/2024.
//

#ifndef ESP32CODE_V3_FLASHSTORAGE_H
#define ESP32CODE_V3_FLASHSTORAGE_H

#include <Arduino.h>


boolean flashStorageInit();

boolean hasWifiCredentials();

boolean setHasWifiCredentials(boolean hasCredentials);

boolean beenOnboarded();

boolean setOnboarded(boolean onboarded);

boolean setWifiCredentials(String newSsid, String newPassword);

String getWifiSsid();

String getWifiPassword();

void clearWifiCredentials();

boolean setSetupComplete(boolean setupComplete);

boolean isSetupComplete();

#endif //ESP32CODE_V3_FLASHSTORAGE_H
