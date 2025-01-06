// SpaceOna Washr ESP32 code V1

// Imports
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <NetworkClientSecure.h>
#include <HTTPClient.h>
#include "wifi_secrets.h"

#define FIRMWARE_VERSION "0.1"

// Setting up the wifi details
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

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

const char *serverName = "https://api.spaceona.com/update/lafayette.edu/watsonhall/washer/0/false?token=NpLvwbWzkgrpq2UZem9TbfN4s6gcBTiNuaoqA3Ap9S9csrEp";

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

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}

// Setting up the timers for the state machine
int timer1 = 0;
int timer2 = millis();
int period = 3000;

void loop() {
  // put your main code here, to run repeatedly:
  timer2 = millis();

  if ((timer2 - timer1) >= period) {
    tickFunction(mpu, client, https, led_1);
    timer1 = timer2;
  }
}

// Setting the states for the tick function
enum States {
  Start,
  Transmit
} Sensor_State;

// Setting up the tick function
void tickFunction(Adafruit_MPU6050 &mpu, NetworkClientSecure &client, HTTPClient &https, int &led_1) {

  // Transitions
  switch (Sensor_State)
  {
  case Start:
    Sensor_State = Transmit;
    break;
  case Transmit:
    Sensor_State = Transmit;
    break;
  default:
    Sensor_State = Start;
    break;
  }

  // State logic
  switch (Sensor_State)
  {
  case Start:
    // State logic would go here if there was any
    break;
  case Transmit:

    digitalWrite(led_1, LOW);

    //Serial.println("Entered Transmit stage!");
    // Checking to make sure the wifi is configured
    if (WiFi.status() != WL_CONNECTED) {
      while (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWifi is offline, trying to reconnect...");
        WiFi.reconnect();
        delay(500);
      }
      Serial.println("\nWifi has reconnected");
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());
    } else {
      
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());

      // Transmitting the data
      Serial.println("Starting HTTPS connection...");
      if (!https.begin(client, serverName)) {
        Serial.println("Failed to start HTTPS connection");
        return;
      }
      https.addHeader("Content-Type", "application/json");
      String httpsPostData = get_mpu_data(mpu);
      Serial.println("Sending POST request...");
      https.setTimeout(5000); //Sets https timeout
      int httpsResponseCode = https.POST(httpsPostData);
      Serial.print("HTTPS Response code: ");
      Serial.println(httpsResponseCode);
      if (httpsResponseCode <= 0) {
        Serial.println("Failed to send POST request");
        digitalWrite(led_1, LOW);
        Serial.println(https.errorToString(httpsResponseCode)); // Print detailed error message
        return;
      } else {
        digitalWrite(led_1, HIGH);
      }
      Serial.println("");
      
    }
    break;
  default:
    break;
  }
}

String get_mpu_data(Adafruit_MPU6050 &mpu) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  char buffer[200]; // Adjust the size as needed
  snprintf(buffer, sizeof(buffer), "{\"accelerometer\":{\"z\":%f,\"y\":%f,\"x\":%f},\"gyroscope\":{\"z\":%f,\"y\":%f,\"x\":%f}}", a.acceleration.z, a.acceleration.y, a.acceleration.x, g.gyro.z, g.gyro.y, g.gyro.x);
  String dataJson = String(buffer);
  Serial.println(dataJson);
  return dataJson;
}
