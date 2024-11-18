//
// Created by leslier on 8/17/2024.
//

#include <Arduino.h>
#include <preferences.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "flashStorage.h"

//TODO write better comments for how these methods work instead of what they do
/**
 * Initializes the Flash Storage to store credentials
 * @return Returns true if the storage is successfully initialized
 */
boolean flashStorageInit(){
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");

    boolean flashInit = flashStorage.isKey("nvsInit");

    if(!flashInit){
        //TODO update this to store client key as well
        flashStorage.end();
        flashStorage.begin("creds", false, "nvs");
        flashStorage.putBool("hasCreds", false);
        flashStorage.putString("ssid", "");
        flashStorage.putString("password", "");
        flashStorage.putBool("nvsInit", true);
    }
    flashStorage.end();
    return true;
}

/**
 * Checks if the board has wifi credentials in the flash storage
 * @return Returns true if the board has wifi credentials stored
 */
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

/**
 * Writes if the board has wifi credentials to flash
 * @param hasCredentials The value to write
 * @return Returns true if it successfully writes to flash
 */
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

/**
 * Writes new wifi credentials to the flash
 * @param newSsid The new SSID to write to flash
 * @param newPassword The new password to write to flash
 * @return Returns true if it successfully writes to flash
 */
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

/**
 * Gets the wifi SSID stored in flash
 * @return The SSID stored in flash
 */
String getWifiSsid() {
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");
    String storedSsid = flashStorage.getString("ssid", "");
    flashStorage.end();
    return storedSsid;
}

/**
 * Gets the wifi password stored in flash
 * @return The wifi password stored in flash
 */
String getWifiPassword() {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");
    String storedPassword = flashStorage.getString("password", "");
    flashStorage.end();
    return storedPassword;
}

/**
 * Clears all the flash for the wifi credentials
 */
void clearWifiCredentials() {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");
    flashStorage.clear();
    flashStorage.putBool("hasCreds", false);
    flashStorage.end();
}

/**
 * Writes if the board has finished setup to the flash
 * @param setupComplete A boolean saying if the board has been setup to write to flash
 * @return Returns true if it successfully writes to flash
 */
boolean setSetupComplete(boolean setupComplete) {
    flashStorage.end();
    flashStorage.begin("creds", false, "nvs");
    flashStorage.putBool("setupComplete", setupComplete);
    flashStorage.end();
    return true;
}

/**
 * Checks the flash to see if the board has been set up
 * @return Returns true if the board has been setup according to the flash
 */
boolean isSetupComplete() {
    flashStorage.end();
    flashStorage.begin("creds", true, "nvs");
    boolean setupComplete = flashStorage.getBool("setupComplete", false);
    flashStorage.end();
    return setupComplete;
}