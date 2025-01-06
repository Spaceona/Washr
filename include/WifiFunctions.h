#ifndef WIFI_FUNCTION_H
#define WIFI_FUNCTION_H


#include <Arduino.h>
#include <ezTime.h>



/**
 * Initializes the wifi
 * @param server_name The name of the server to connect to
 * @param https The HTTPS client to use for the connection
 * @return Returns true if the wifi is successfully initialized, and false if there is a failure
 */
boolean wifi_init(String server_name, HTTPClient& https);

/**
 * Connects to the wifi
 */
void wifiConnect();

/**
 * Authenticates the board with the server
 * @return Returns the HTTP code of the request, or -1 if there is an error
 */
int serverAuth();

/**
 * Sends the machine status to the server
 * @param currentMachineStatus The current status of the machine
 * @return Returns the HTTP code of the request, or -1 if there is an error
 */
int machineStatusUpdate(boolean currentMachineStatus);

/**
 * Checks the server for a new firmware version
 * @param updateFirmware The version of the firmware to check for
 */
void otaUpdate(String updateFirmware);

/**
 * Onboards the board to the server
 * @return Returns the HTTP code of the request, or -1 if there is an error
 */
int onboardBoard();

/**
 * Adds the authorization headers to the HTTP client
 * @param callbackClient The client to add the headers to
 */
void requestCallback(HTTPClient* callbackClient);

/**
 * Syncs the clock to the NTP server
 */
void setClock();

/**
 * Checks the firmware version of the board
 */
void firmwareCheck();

/**
 * Sets the time for the firmware update
 * @return //TODO add documentation
 */
time_t firmwareUpdateTime();

/**
 * Gets the latest firmware version from the server
 * @return Returns the latest firmware version, or "error" if there is an error
 */
String latestFirmware();

/**
 * Updates the machine status every x minutes
 * @param minutePeriod The period to update the machine status
 * @return Returns the time to update the machine status
 */
time_t heartbeatUpdateTime(int minutePeriod);

/**
 * Sends a heartbeat to the server
 */
void sendHeartbeat();

#endif
