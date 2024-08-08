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


void wifi_init(String server_name, HTTPClient& https) {
    // Setting up the wifi
    WiFi.mode(WIFI_STA); // Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("");

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Get MAC address of the WiFi station interface
    mac_address = WiFi.macAddress();
    Serial.println("Mac Address: ");
    Serial.println(mac_address);

    // Installing the certificate for HTTPS
    //client.setCACert(test_root_ca);

    //Not sure why the cert isnt working, but it does work insecurely
    client.setInsecure();

    //Serial.println(test_root_ca);

    //Testing if the certificate is installed correctly
    Serial.println("Testing certificate and connection to server");
    if (!client.connect("api.spaceona.com", 3000)) {
        Serial.println("Connection failed");
        return;
    }
    else {
        Serial.println("Connection successful");
    }
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

boolean serverAuth(){
    String auth_server;
    JsonDocument device_data;
    JsonDocument auth_data;

    endpoint = "/auth/device";
    auth_server = server_name + endpoint;
    Serial.println("Auth server: " + auth_server);
    Serial.println("Starting HTTPS connection...");
    if (!authClient.begin(testClient, auth_server)) {
        Serial.println("Failed to start HTTPS connection");
        return false;
    }


    device_data["mac"] = mac_address;
    device_data["firmwareVersion"] = FIRMWARE_VERSION;
    device_data["clientKey"] = clientKey;

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
                return false;
            }

            const char* message = auth_data["message"]; // "Authenticated"
            jwt = String(auth_data["jwt"].as<const char*>()); //converting to a string
            Serial.println("JWT: " + jwt);
            Serial.println("");
            authenticated = true;
            digitalWrite(led_1, HIGH);
            delay(50);
            digitalWrite(led_1, LOW);
            return true;
        } else if (httpCode == 400){
            Serial.println("Unauthorized");
            authenticated = false;
            return false;
        }
    }
    else{
        Serial.println("Error on HTTP request");
        return false;
    }

    return true;
}

HTTPClient statusClient;

void machineStatusUpdate(boolean machineStatus){
    //Setting up the endpoint
    endpoint = "/update/" + clientName + "/" + building + "/" + type + "/" + id;
    String statusServer = server_name + endpoint;
    Serial.println("Status server: " + statusServer);

    //Testing server connection
    Serial.println("Starting HTTPS connection...");
    if (!statusClient.begin(testClient, statusServer)) {
        Serial.println("Failed to start HTTPS connection");
        return;
    }
    Serial.println("Connected to the server");

    //Setting up the data to be sent
    JsonDocument statusData;
    statusData["firmwareVersion"] = FIRMWARE_VERSION;
    statusData["status"] = machineStatus;
    statusData["confidence"] = detectionConfidence;
    String statusDataString;
    serializeJson(statusData, statusDataString);
    Serial.println("Data to be sent: " + statusDataString);
    Serial.println("JWT: " + jwt);
    //Adding headers
    String authHeader = "Bearer " + jwt;
    statusClient.addHeader("Authorization", authHeader);
    statusClient.addHeader("Content-Type", "application/json");

    //Posting
    int httpCode = statusClient.POST(statusDataString);

    Serial.print("HTTP Code: ");
    Serial.println(httpCode);
    if(httpCode > 0){
        String responseBody = authClient.getString();
        Serial.println("Response body: " + responseBody);
        if(httpCode == 200){
            Serial.println("Status updated successfully");
            digitalWrite(led_1, HIGH);
            delay(100);
            digitalWrite(led_1, LOW);
            return;
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
    }
    else{
        Serial.println("Error on HTTP request");
    }
}

//TODO change this to a secure connection when it is implemented on the server
void ota_update(WiFiClient ota_client, String ota_server_url, uint16_t ota_port, String ota_firmware_location) {
    Serial.println("Starting OTA update check");

    t_httpUpdate_return ret = httpUpdate.update(ota_client, ota_server_url, ota_port, ota_firmware_location);

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
time_t firmwareUpdateTime() {
    setClock();
    time_t now = myTimezone.now();

    tmElements_t tm;
    breakTime(now, tm);

    // Set the time to 7 PM tomorrow
    //tm.Hour = 19;  // 7 PM
    tm.Minute += 1; // 0 minutes //Testing every minute to check if it is working
    tm.Second = 0; // 0 seconds
    //tm.Day += 1;   // Move to the next day

    //Return the new time
    return makeTime(tm);
}

//Callback function for ezTime events to check for firmware updates and if there are any, update the firmware
void firmwareCheck() {
    time_t firmwareTime = firmwareUpdateTime();
    Serial.println("Event callback reached");
    myTimezone.setEvent(firmwareCheck, firmwareTime);
}
