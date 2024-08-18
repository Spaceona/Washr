//
// Created by leslier on 8/17/2024.
//

#include <Arduino.h>
#include <preferences.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "flashStorage.h"

void flashStorageInit() {
    flashStorage.begin("wifiCreds", false);
    flashStorage.begin("hasCreds", false);
}

boolean hasWifiCredentials() {
    flashStorage.begin("hasCreds", false);
    boolean hasCredentials =  flashStorage.getBool("hasCreds", false);
    Serial.println("Has credentials: " + String(hasCredentials));
    return hasCredentials;
}

void setHasWifiCredentials(boolean hasCredentials) {
    flashStorage.begin("hasCreds", false);
    flashStorage.putBool("hasCreds", hasCredentials);
}

void setWifiCredentials(String newSsid, String newPassword) {
    flashStorage.begin("wifiCreds", false);
    flashStorage.putString("ssid", newSsid);
    flashStorage.putString("password", newPassword);
    flashStorage.begin("hasCreds", false);
    flashStorage.putBool("hasCreds", true);
}

String getWifiSsid() {
    flashStorage.begin("wifiCreds", false);
    String storedSsid = flashStorage.getString("ssid", "");
    return ssid;
}

String getWifiPassword() {
    flashStorage.begin("wifiCreds", false);
    String storedPassword = flashStorage.getString("password", "");
    return storedPassword;
}

void clearWifiCredentials() {
    flashStorage.begin("wifiCreds", false);
    flashStorage.clear();
    flashStorage.begin("hasCreds", false);
    flashStorage.putBool("hasCreds", false);
}

void flashStorageClose(){
    flashStorage.end();
}