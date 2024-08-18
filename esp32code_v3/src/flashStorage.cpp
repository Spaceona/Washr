//
// Created by leslier on 8/17/2024.
//

#include <Arduino.h>
#include <preferences.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "flashStorage.h"

Preferences flashStorage;

boolean hasWifiCredentials() {
    flashStorage.begin("hasWifiCredentials", false);
    boolean hasCredentials =  flashStorage.getBool("hasWifiCredentials", false);
    flashStorage.end();
    return hasCredentials;
}

void setHasWifiCredentials(boolean hasCredentials) {
    flashStorage.begin("hasWifiCredentials", false);
    flashStorage.putBool("hasWifiCredentials", hasCredentials);
    flashStorage.end();
}

void setWifiCredentials(String newSsid, String newPassword) {
    flashStorage.begin("wifiCredentials", false);
    flashStorage.putString("ssid", newSsid);
    flashStorage.putString("password", newPassword);
    flashStorage.begin("hasWifiCredentials", false);
    flashStorage.putBool("hasWifiCredentials", true);
    flashStorage.end();
}

String getWifiSsid() {
    flashStorage.begin("wifiCredentials", false);
    String storedSsid = flashStorage.getString("ssid", "");
    flashStorage.end();
    return ssid;
}

String getWifiPassword() {
    flashStorage.begin("wifiCredentials", false);
    String storedPassword = flashStorage.getString("password", "");
    flashStorage.end();
    return storedPassword;
}

void clearWifiCredentials() {
    flashStorage.begin("wifiCredentials", false);
    flashStorage.clear();
    flashStorage.end();
    flashStorage.begin("hasWifiCredentials", false);
    flashStorage.putBool("hasWifiCredentials", false);
    flashStorage.end();
}