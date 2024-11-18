#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
//Might want to move to the HttpsOTAUpdate library at some point since its part of the official arduino library
#include <ezTime.h>
#include "wifi_secrets.h"

// Setting up the wifi details
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
String mac_address;

//Url for the firmware location
//const char *firmware_url =

// Setting up the server certificate
const char *test_root_ca =
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDpDCCA0ugAwIBAgIQPWjtzWvFvUUREnPm431e2jAKBggqhkjOPQQDAjA7MQsw\n" \
    "CQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMQwwCgYD\n" \
    "VQQDEwNXRTEwHhcNMjQwNjI3MjIwNjAxWhcNMjQwOTI1MjIwNjAwWjAXMRUwEwYD\n" \
    "VQQDEwxzcGFjZW9uYS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATJ7TVj\n" \
    "e81auJfB2U41VuDszt1yOY1H5h3f4auVGTwSPo8Q9SeMfZJafSyF5fujY7gvkZr+\n" \
    "6/CbpnQaRaiOmYz7o4ICUzCCAk8wDgYDVR0PAQH/BAQDAgeAMBMGA1UdJQQMMAoG\n" \
    "CCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFN3jz3qE62YQqApj0dyW\n" \
    "fvHa6KL2MB8GA1UdIwQYMBaAFJB3kjVnxP+ozKnme9mAeXvMk/k4MF4GCCsGAQUF\n" \
    "BwEBBFIwUDAnBggrBgEFBQcwAYYbaHR0cDovL28ucGtpLmdvb2cvcy93ZTEvUFdn\n" \
    "MCUGCCsGAQUFBzAChhlodHRwOi8vaS5wa2kuZ29vZy93ZTEuY3J0MCcGA1UdEQQg\n" \
    "MB6CDHNwYWNlb25hLmNvbYIOKi5zcGFjZW9uYS5jb20wEwYDVR0gBAwwCjAIBgZn\n" \
    "gQwBAgEwNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2MucGtpLmdvb2cvd2UxL3Q0\n" \
    "WV90UzRvUTlBLmNybDCCAQIGCisGAQQB1nkCBAIEgfMEgfAA7gB1AHb/iD8KtvuV\n" \
    "UcJhzPWHujS0pM27KdxoQgqf5mdMWjp0AAABkFvy4I8AAAQDAEYwRAIfauowiX8k\n" \
    "x2ORhFZUzIIkUaPn2OmFnSjT0CF3sr9ZNAIhAPr4Y1bKpMdS1ANqoLk602nLtjgz\n" \
    "RfvbIfDmqYPPYVWOAHUAPxdLT9ciR1iUHWUchL4NEu2QN38fhWrrwb8ohez4ZG4A\n" \
    "AAGQW/LkVQAABAMARjBEAiB9ug1AIINZBkAuTDeEULSfDqzD/YjL9qb2ZADJ6VwR\n" \
    "FAIgKygXNOHYW11mSR1hFhTKeOt+3P1ec+7GzyUvo6PvtoEwCgYIKoZIzj0EAwID\n" \
    "RwAwRAIgTrgBxN5UA+9CZZ+zNJ1Vsiq0qv7+EIJ91uI94HbszY4CIDpPQmuYk0VG\n" \
    "RoMNAmeLm32ELp1pxWsJbeSq2sMJmWv4\n" \
    "-----END CERTIFICATE-----\n";



void wifi_init(const char* server_name, WiFiClientSecure &client, HTTPClient &https){
  // Setting up the wifi
  WiFi.mode(WIFI_STA); // Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

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
  if (!client.connect("api.spaceona.com", 443)) {
    Serial.println("Connection failed");
    return;
  } else {
    Serial.println("Connection successful");
  }

}

//I want to add a feature where I compare the current firmware version to the remote firmware version and only update if it is bigger, but part of that is server side
//I might also want to have it only update at like 3 in the morning instead of doing it randomly every x hours
void ota_update(WiFiClient ota_client, String ota_server_url, uint16_t ota_port, String ota_firmware_location){
  Serial.println("Starting OTA update check");

  t_httpUpdate_return ret = httpUpdate.update(ota_client, ota_server_url, ota_port, ota_firmware_location);

  switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
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

// Set time via NTP, will be used for OTA sync at an offtime. Look into ezTime "setEvent()"
void setClock(Timezone &time_zone, String timezone_name) {
  waitForSync();

	Serial.println("UTC: " + UTC.dateTime());
  time_zone.setLocation(timezone_name);
  Serial.print("Local time: ");
  time_zone.setLocation(timezone_name);
  Serial.println(time_zone.dateTime());
}
