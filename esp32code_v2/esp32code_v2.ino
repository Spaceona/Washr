// SpaceOna Washr ESP32 code V1

// Imports
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <NetworkClientSecure.h>
#include <HTTPClient.h>
#include "wifi_secrets.h"
#include "MPUFunctions.h"
#include "TickFunction.h"

#define FIRMWARE_VERSION "0.2"

// Setting up the wifi details
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
String mac_address;

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


// Setting up the MPU
Adafruit_MPU6050 mpu;

// Setting up the LED
int led_1 = D10;

// Setting up the wifi client for making HTTPS requests
NetworkClientSecure client;
HTTPClient https;

const char *server_name = "https://api.spaceona.com/update/lafayette.edu/watsonhall/washer/0/false?token=NpLvwbWzkgrpq2UZem9TbfN4s6gcBTiNuaoqA3Ap9S9csrEp";

void setup(){

  // Setting up the serial
  Serial.begin(115200);
  delay(1000);

  // Setting up the LED
  pinMode(led_1, OUTPUT);

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

  // Setting up the MPU
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //Initializing MPU
  mpu_init(mpu, MPU6050_RANGE_8_G, MPU6050_RANGE_500_DEG, MPU6050_BAND_5_HZ);

}

// Setting up the timers for the state machine
int timer1 = 0;
int timer2 = millis();
int period = 3000;

void loop() {
  // put your main code here, to run repeatedly:
  timer2 = millis();

  if ((timer2 - timer1) >= period) {
    tickFunction(mpu, server_name, client, https, led_1);
    timer1 = timer2;
  }
}