//
// Created by leslier on 8/17/2024.
//

#ifndef ESP32CODE_V3_FLASHSTORAGE_H
#define ESP32CODE_V3_FLASHSTORAGE_H

#include <Arduino.h>

/**
 * Initializes the Flash Storage to store credentials
 * @return Returns true if the storage is successfully initialized
 */
boolean flashStorageInit();

/**
 * Checks if the board has wifi credentials in the flash storage
 * @return Returns true if the board has wifi credentials stored
 */
boolean hasWifiCredentials();

/**
 * Writes if the board has wifi credentials to flash
 * @param hasCredentials The value to write
 * @return Returns true if it successfully writes to flash
 */
boolean setHasWifiCredentials(boolean hasCredentials);

/**
 * Writes new wifi credentials to the flash
 * @param newSsid The new SSID to write to flash
 * @param newPassword The new password to write to flash
 * @return Returns true if it successfully writes to flash
 */
boolean setWifiCredentials(String newSsid, String newPassword);

/**
 * Gets the wifi SSID stored in flash
 * @return The SSID stored in flash
 */
String getWifiSsid();

/**
 * Gets the wifi password stored in flash
 * @return The wifi password stored in flash
 */
String getWifiPassword();

/**
 * Clears all the flash for the wifi credentials
 */
void clearWifiCredentials();

/**
 * Writes if the board has finished setup to the flash
 * @param setupComplete A boolean saying if the board has been setup to write to flash
 * @return Returns true if it successfully writes to flash
 */
boolean setSetupComplete(boolean setupComplete);

/**
 * Checks the flash to see if the board has been set up
 * @return Returns true if the board has been setup according to the flash
 */
boolean isSetupComplete();

#endif //ESP32CODE_V3_FLASHSTORAGE_H
