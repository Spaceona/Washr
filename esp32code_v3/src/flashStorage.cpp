//
// Created by leslier on 8/17/2024.
//

#include <Arduino.h>
#include <preferences.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "flashStorage.h"

boolean flashStorageInit(){
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");

    boolean flashInit = flashStorage.isKey("nvsInit");

    if(!flashInit){
        flashStorage.end();
        flashStorage.begin("creds", false, "nvs");
        flashStorage.putBool("hasCreds", false);
        flashStorage.putBool("onboarded", false);
        flashStorage.putString("ssid", "");
        flashStorage.putString("password", "");
        flashStorage.putBool("nvsInit", true);
    }
    flashStorage.end();
    return true;
}

boolean hasWifiCredentials() {
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");

    if(flashStorage.getBool("nvsInit", false) == false){
        return false;
    }

    boolean hasCredentials =  flashStorage.getBool("hasCreds", false);
    Serial.println("Has credentials: " + String(hasCredentials));
    flashStorage.end();
    return hasCredentials;
}

boolean setHasWifiCredentials(boolean hasCredentials) {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");

    if(flashStorage.getBool("nvsInit", false) == false){
        return false;
    }

    flashStorage.putBool("hasCreds", hasCredentials);
    flashStorage.end();
    return true;
}

boolean beenOnboarded() {
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");

    if(flashStorage.getBool("nvsInit", false) == false){
        return false;
    }

    boolean beenOnboarded =  flashStorage.getBool("onboarded", false);
    Serial.println("Has credentials: " + String(beenOnboarded));
    flashStorage.end();
    return beenOnboarded;
}

boolean setOnboarded(boolean onboarded) {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");

    if(flashStorage.getBool("nvsInit", false) == false){
        return false;
    }

    flashStorage.putBool("onboarded", onboarded);
    flashStorage.end();
    return true;
}

boolean setWifiCredentials(String newSsid, String newPassword) {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");

    if(flashStorage.getBool("nvsInit", false) == false){
        return false;
    }

    flashStorage.putString("ssid", newSsid);
    flashStorage.putString("password", newPassword);
    flashStorage.putBool("hasCreds", true);

    //Debug print statements
    // Serial.println("SSID: " + flashStorage.getString("ssid", ""));
    // Serial.println("Password: " + flashStorage.getString("password", ""));
    // Serial.println("Has credentials: " + String(flashStorage.getBool("hasCreds", false)));

    flashStorage.end();
    return true;
}

String getWifiSsid() {
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");
    String storedSsid = flashStorage.getString("ssid", "");
    flashStorage.end();
    return storedSsid;
}

String getWifiPassword() {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");
    String storedPassword = flashStorage.getString("password", "");
    flashStorage.end();
    return storedPassword;
}

void clearWifiCredentials() {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");
    flashStorage.clear();
    flashStorage.putBool("hasCreds", false);
    flashStorage.end();
}

boolean setSetupComplete(boolean setupComplete) {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");
    flashStorage.putBool("setupComplete", setupComplete);
    flashStorage.end();
    return true;
}

boolean isSetupComplete() {
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");
    boolean setupComplete = flashStorage.getBool("setupComplete", false);
    flashStorage.end();
    return setupComplete;
}