// Created by Robbie Leslie
// Modified by

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h> //Need to use for insecure local testing
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ArduinoJson.h>
//Might want to move to the HttpsOTAUpdate library at some point since its part of the official arduino library
#include <ezTime.h>
#include "globals.h"
#include "wifi_secrets.h"
#include "WifiFunctions.h"
#include "flashStorage.h"



// Setting up the server certificate
const char* test_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDpDCCA0ugAwIBAgIQPWjtzWvFvUUREnPm431e2jAKBggqhkjOPQQDAjA7MQsw\n"
    "CQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMQwwCgYD\n"
    "VQQDEwNXRTEwHhcNMjQwNjI3MjIwNjAxWhcNMjQwOTI1MjIwNjAwWjAXMRUwEwYD\n"
    "VQQDEwxzcGFjZW9uYS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATJ7TVj\n"
    "e81auJfB2U41VuDszt1yOY1H5h3f4auVGTwSPo8Q9SeMfZJafSyF5fujY7gvkZr+\n"
    "6/CbpnQaRaiOmYz7o4ICUzCCAk8wDgYDVR0PAQH/BAQDAgeAMBMGA1UdJQQMMAoG\n"
    "CCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFN3jz3qE62YQqApj0dyW\n"
    "fvHa6KL2MB8GA1UdIwQYMBaAFJB3kjVnxP+ozKnme9mAeXvMk/k4MF4GCCsGAQUF\n"
    "BwEBBFIwUDAnBggrBgEFBQcwAYYbaHR0cDovL28ucGtpLmdvb2cvcy93ZTEvUFdn\n"
    "MCUGCCsGAQUFBzAChhlodHRwOi8vaS5wa2kuZ29vZy93ZTEuY3J0MCcGA1UdEQQg\n"
    "MB6CDHNwYWNlb25hLmNvbYIOKi5zcGFjZW9uYS5jb20wEwYDVR0gBAwwCjAIBgZn\n"
    "gQwBAgEwNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2MucGtpLmdvb2cvd2UxL3Q0\n"
    "WV90UzRvUTlBLmNybDCCAQIGCisGAQQB1nkCBAIEgfMEgfAA7gB1AHb/iD8KtvuV\n"
    "UcJhzPWHujS0pM27KdxoQgqf5mdMWjp0AAABkFvy4I8AAAQDAEYwRAIfauowiX8k\n"
    "x2ORhFZUzIIkUaPn2OmFnSjT0CF3sr9ZNAIhAPr4Y1bKpMdS1ANqoLk602nLtjgz\n"
    "RfvbIfDmqYPPYVWOAHUAPxdLT9ciR1iUHWUchL4NEu2QN38fhWrrwb8ohez4ZG4A\n"
    "AAGQW/LkVQAABAMARjBEAiB9ug1AIINZBkAuTDeEULSfDqzD/YjL9qb2ZADJ6VwR\n"
    "FAIgKygXNOHYW11mSR1hFhTKeOt+3P1ec+7GzyUvo6PvtoEwCgYIKoZIzj0EAwID\n"
    "RwAwRAIgTrgBxN5UA+9CZZ+zNJ1Vsiq0qv7+EIJ91uI94HbszY4CIDpPQmuYk0VG\n"
    "RoMNAmeLm32ELp1pxWsJbeSq2sMJmWv4\n"
    "-----END CERTIFICATE-----\n";


boolean wifi_init(String server_name, HTTPClient& https) {
    // Setting up the wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    //TODO add a timeout for the wifi connection

    // Define a timeout period (e.g., 10 seconds)
    //const unsigned long wifiTimeout = 1000 * 60 * 5; // 5 minutes
    const unsigned long wifiTimeout = 30000;

// Record the start time
    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED) {
        // Check if the timeout period has been exceeded
        if (millis() - startTime >= wifiTimeout) {
            Serial.println("WiFi connection timed out");
            // Handle the timeout case (e.g., reset the device, retry, etc.)
            return false;
        }
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");

    Serial.println("Signal strength (RSSI): " + String(WiFi.RSSI()));

    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Get MAC address of the WiFi station interface
    mac_address = WiFi.macAddress();
    Serial.println("Mac Address: ");
    Serial.println(mac_address);

    //Printing the firmware version
    Serial.println("Firmware version: " + FIRMWARE_VERSION);

    // Installing the certificate for HTTPS
    //client.setCACert(test_root_ca);

    //Not sure why the cert isnt working, but it does work insecurely
    client.setInsecure();

    //Serial.println(test_root_ca);

    //TODO use secure client
    //Testing if the certificate is installed correctly
    //Serial.println("Testing certificate and connection to server");
    if (!client.connect("10.1.1.194", 3001)) {
        Serial.println("Connection failed");
        //TODO change this later when it becomes a problem
        return true;
    }
    else {
        Serial.println("Connection successful");
    }
    return true;
}

void wifiConnect(){
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWifi is offline, trying to reconnect...");
        WiFi.reconnect();
        delay(500);
    }
    Serial.println("\nWifi has reconnected");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

//Setting up the HTTPS information
WiFiClient testClient;
HTTPClient authClient;

int serverAuth(){
    String auth_server;
    JsonDocument device_data;
    JsonDocument auth_data;

    endpoint = "/auth/device";
    auth_server = server_name + endpoint;
    //Serial.println("Auth server: " + auth_server);
    //Serial.println("Starting HTTPS connection...");
    if (!authClient.begin(client, auth_server)) {
        Serial.println("Failed to start HTTPS connection");
        return false;
    }


    device_data["mac_address"] = mac_address;
    device_data["firmware_version"] = FIRMWARE_VERSION;
    device_data["client_key"] = clientKey;

    String device_data_string;
    serializeJson(device_data, device_data_string);
    //Serial.println(device_data_string);

    Serial.println("Sending POST request...");
    authClient.addHeader("Content-Type", "application/json");
    int httpCode = authClient.POST(device_data_string);

    Serial.print("HTTP Code: ");
    Serial.println(httpCode);

    if(httpCode > 0){
        String responseBody = authClient.getString();
        Serial.println("Response body: " + responseBody);
        if(httpCode == 200){
            DeserializationError error = deserializeJson(auth_data, responseBody);

            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                authClient.end();
                return -1;
            } else if(!auth_data["Token"].is<const char*>()){
                Serial.println("Bad response from server");
                authClient.end();
                return -1;
            }

            const char* message = auth_data["message"]; // "Authenticated"
            jwt = String(auth_data["Token"].as<const char*>()); //converting to a string
            //Serial.println("JWT: " + jwt);
            //Serial.println("");
            authenticated = true;
            digitalWrite(led_1, HIGH);
            delay(50);
            digitalWrite(led_1, LOW);
            authClient.end();
            return 200;
        } else if (httpCode == 400){
            Serial.println("Unauthorized");
            authClient.end();
            authenticated = false;
            return 400;
        }
    } else{
        Serial.println("Error on HTTP request");
        authClient.end();
        return -1;
    }
    authClient.end();
    return -1;
}

HTTPClient statusClient;

int machineStatusUpdate(boolean currentMachineStatus){
    //Setting up the endpoint
    endpoint = "/status/update";
    String statusServer = server_name + endpoint;
    //Serial.println("Status server: " + statusServer);

    //Testing server connection
    Serial.println("Starting HTTPS connection...");
    if (!statusClient.begin(client, statusServer)) {
        Serial.println("Failed to start HTTPS connection");
        statusClient.end();
        return -1;
    }
    Serial.println("Connected to the server");

    //Setting up the data to be sent
    JsonDocument statusData;
    statusData["mac_address"] = mac_address;
    statusData["firmwareVersion"] = FIRMWARE_VERSION;
    statusData["status"] = currentMachineStatus;
    statusData["confidence"] = detectionConfidence;
    String statusDataString;
    serializeJson(statusData, statusDataString);
    //Serial.println("Data to be sent: " + statusDataString);
    //Serial.println("JWT: " + jwt);
    //Adding headers
    String authHeader = "Bearer " + jwt;
    statusClient.addHeader("Authorization", authHeader);
    statusClient.addHeader("Content-Type", "application/json");

    //Posting
    int httpCode = statusClient.POST(statusDataString);

    Serial.print("Status HTTP Code: ");
    Serial.println(httpCode);
    String responseBody = statusClient.getString();
    Serial.println("Response body: " + responseBody);
    statusClient.end();
    if(httpCode > 0){
        if(httpCode == 200){
            Serial.println("Status updated successfully");
            digitalWrite(led_1, HIGH);
            delay(100);
            digitalWrite(led_1, LOW);
        }
        else if (httpCode == 401){
            Serial.println("Unauthorized");
            authenticated = false;
        } else if(httpCode == 400){
            Serial.println("Bad request");
            //TODO check client and building assignment if bad request
        } else if(httpCode == 500){
            Serial.println("Internal server error");
        } else {
            Serial.println("Failed to update machine status: Unknown error");
        }
        return httpCode;
    } else{
        Serial.println("Error on HTTP request");
    }
    return -1;
}


//TODO swap the testclient to just one client
int onboardBoard(){
    Serial.println("Onboarding board");
    //Setting up the endpoint
    endpoint = "/onboard/board";
    String onboardServer = server_name + endpoint;
    Serial.println("Onboard server: " + onboardServer);

    //Testing server connection
    Serial.println("Starting HTTPS connection...");
    if (!statusClient.begin(client, onboardServer)) {
        Serial.println("Failed to start HTTPS connection");
        statusClient.end();
        return -1;
    }

    //Setting up the data
    JsonDocument onboardData;
    onboardData["mac_address"] = mac_address;
    onboardData["client_name"] = clientName;
    onboardData["client_key"] = clientKey;
    String onboardDataString;
    serializeJson(onboardData, onboardDataString);
    Serial.println("Data to be sent: " + onboardDataString);
    //Adding headers
    //TODO check the headers

    //Posting
    int httpCode = statusClient.POST(onboardDataString);

    Serial.print("Status HTTP Code: ");
    Serial.println(httpCode);
    String responseBody = statusClient.getString();
    //Serial.println("Response body: " + responseBody);
    statusClient.end();
    if(httpCode > 0){
        if(httpCode == 200){
            Serial.println("Onboarded board correctly");
            digitalWrite(led_2, HIGH);
            delay(100);
            digitalWrite(led_2, LOW);
            setupComplete = true;
            setSetupComplete(setupComplete);
        }
        else if (httpCode == 401){
            Serial.println("Unauthorized");
            setupComplete = false;
            setSetupComplete(setupComplete);
        } else if(httpCode == 400){
            Serial.println("Bad request");
            setupComplete = false;
            setSetupComplete(setupComplete);
        } else if(httpCode == 500){
            Serial.println("Internal server error");
            setupComplete = false;
            setSetupComplete(setupComplete);
        } else {
            Serial.println("Failed to update machine status: Unknown error");
            setupComplete = false;
            setSetupComplete(setupComplete);
        }
        return httpCode;
    } else {
        Serial.println("Error on HTTP request");
    }
    setupComplete = false;
    setSetupComplete(setupComplete);
    return -1;
}

//TODO change this to a secure connection when it is implemented on the server
void otaUpdate(String updateFirmware) {
    Serial.println("Starting OTA update check");

    String firmwareEndpoint = "/firmware/file/" + updateFirmware;
    String firmwareServer = server_name + firmwareEndpoint;

    Serial.println("Firmware server: " + firmwareServer);
    t_httpUpdate_return ret = httpUpdate.update(client,  firmwareServer, requestCallback);

    switch (ret) {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(),
                      httpUpdate.getLastErrorString().c_str());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        delay(1000); // Wait a second and restart
        ESP.restart();
        break;
    }
}

//This function is used to add the authentication header to the OTA update request
void requestCallback(HTTPClient* callbackClient){
    callbackClient->addHeader("Authorization", "Bearer " + jwt);
}

// Set time via NTP, will be used for OTA sync at an off time. Look into ezTime "setEvent()"
void setClock() {
    waitForSync();

    Serial.println("UTC: " + UTC.dateTime());
    myTimezone.setLocation(timezone_name);
    Serial.print("Local time: ");
    myTimezone.setLocation(timezone_name);
    Serial.println(myTimezone.dateTime());
}

//Returns 7pm the next day
//TODO change this to have it be a parameter
time_t firmwareUpdateTime() {
    setClock();
    time_t now = myTimezone.now();

    tmElements_t tm;
    breakTime(now, tm);

    // Set the time to 3 AM tomorrow
    tm.Hour = 3;  // 3 Am
    tm.Minute = 0; // 0 minutes
    tm.Second = 0; // 0 seconds
    tm.Day += 1;   // Move to the next day

    //Return the new time
    return makeTime(tm);
}

//Callback function for ezTime events to check for firmware updates and if there are any, update the firmware
void firmwareCheck() {
    time_t firmwareTime = firmwareUpdateTime();
    Serial.println("Event callback reached");
    String latest_Firmware = latestFirmware();

    if(latest_Firmware == "error"){
        Serial.println("Error checking for latest firmware");
        myTimezone.setEvent(firmwareCheck, firmwareTime);
        return;
    }

    int latestMajorVersion = latest_Firmware.substring(0, 1).toInt();
    int latestMinorVersion = latest_Firmware.substring(2, 3).toInt();
    int latestPatchVersion = latest_Firmware.substring(4, 5).toInt();

    if(latestMajorVersion > FIRMWARE_VERSION_MAJOR){
        Serial.println("Major version update available");
        otaUpdate(latest_Firmware);
    } else if(latestMinorVersion > FIRMWARE_VERSION_MINOR && latestMajorVersion >= FIRMWARE_VERSION_MAJOR){
        Serial.println("Minor version update available");
        otaUpdate(latest_Firmware);
    } else if(latestPatchVersion > FIRMWARE_VERSION_PATCH && latestMinorVersion >= FIRMWARE_VERSION_MINOR && latestMajorVersion >= FIRMWARE_VERSION_MAJOR){
        Serial.println("Patch version update available");
        otaUpdate(latest_Firmware);
    } else {
        Serial.println("No updates available");
    }

    myTimezone.setEvent(firmwareCheck, firmwareTime);
}


HTTPClient firmwareClient;

String latestFirmware(){
    Serial.println("Checking for latest firmware version");
    //Setting up the endpoint
    endpoint = "/firmware/latest";
    String firmwareServer = server_name + endpoint;
    //Serial.println("Firmware server: " + firmwareServer);

    //Testing server connection
    Serial.println("Starting HTTPS connection...");
    if (!firmwareClient.begin(client, firmwareServer)) {
        Serial.println("Failed to start HTTPS connection");
        firmwareClient.end();
        return "error";
    }
    Serial.println("Connected to the server");

    //Adding headers
    String authHeader = "Bearer " + jwt;
    firmwareClient.addHeader("Authorization", authHeader);
    firmwareClient.addHeader("Content-Type", "application/json");

    //Getting
    int httpCode = firmwareClient.GET();

    Serial.print("Firmware HTTP Code: ");
    Serial.println(httpCode);
    String responseBody = firmwareClient.getString();
    //Serial.println("Response body: " + responseBody);
    firmwareClient.end();
    if(httpCode > 0){
        if(httpCode == 200){

            //Parsing the response
            JsonDocument firmwareData;
            DeserializationError error = deserializeJson(firmwareData, responseBody);
            if(error){
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                return "error";
            }
            String version = firmwareData["version"];

            Serial.println("Latest firmware version: " + version);
            digitalWrite(led_1, HIGH);
            delay(100);
            digitalWrite(led_1, LOW);
            return version;
        }
        else if (httpCode == 401){
            Serial.println("Unauthorized");
            authenticated = false;
            return "error";
        } else if(httpCode == 400){
            Serial.println("Bad request");
            return "error";
        } else if(httpCode == 500){
            Serial.println("Internal server error");
            return "error";
        } else {
            Serial.println("Failed to update machine status: Unknown error");
            return "error";
        }
    }
    else{
        Serial.println("Error on HTTP request");
        return "error";
    }
}

//Returns X minutes from current time
time_t heartbeatUpdateTime(int minutePeriod) {
    time_t now = myTimezone.now();

    tmElements_t tm;
    breakTime(now, tm);

    // Add minutePeriod minutes to the current time
    tm.Minute += minutePeriod;

    // Handle overflow of minutes
    if (tm.Minute >= 60) {
        tm.Minute -= 60;
        tm.Hour += 1;
    }

    // Handle overflow of hours
    if (tm.Hour >= 24) {
        tm.Hour -= 24;
        tm.Day += 1;
    }

    //Return the new time
    return makeTime(tm);
}

//Sending the heartbeat to the server every x minutes
void sendHeartbeat(){
    time_t heartbeatTime = heartbeatUpdateTime(heartbeatPeriod);
    Serial.println("Event callback reached. Sending heartbeat");

    int returnCode = machineStatusUpdate(machineStatus);

    if(returnCode == 200){
        Serial.println("Heartbeat sent successfully");
        heartbeatSent = true;
    } else {
        Serial.println("Failed to send heartbeat");
        heartbeatSent = false;
    }
    //TODO figure out what to do if the heartbeat isn't sent successfully
}
