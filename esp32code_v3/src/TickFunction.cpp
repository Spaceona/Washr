#include <Arduino.h>

#ifdef SEEEED_XIAO_ESP32C3
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MPUFunctions.h"
#elif CUSTOM_BOARD
#include <ICM42670P.h>
#include "IMUFunctions.h"
#endif

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "globals.h"
#include "TickFunction.h"
#include "WifiFunctions.h"
#include "flashStorage.h"


// Setting up the https client for making HTTPS requests
HTTPClient https;

// Setting the states for the tick function
enum States {
    Start,
    getCreds,
    wifiInit,
    Setup,
    Transmit,
    Authenticate,
    Wait
} Sensor_State;

boolean previousMachineStatus;
int previousHttpCode;
int setupTries = 0;
int setupThreshold = 8; //Can change this depend on how fast the tick function is running
boolean setupFailed = false;

// Setting up the tick function
void tickFunction() {
    // Transitions
    Serial.println("Starting State: " + String(Sensor_State)); //Debugging statement

    // State logic
    switch (Sensor_State) {
        case Start: {
            // State logic would go here if there was any
            Serial.println("Starting");
            previousMachineStatus = false;
            setupComplete = isSetupComplete(); //Reading if the board has already been onboarded
            Serial.println("Setup complete in start: " + String(setupComplete));
            break;
        }
        case getCreds: {
            //TODO Long term this is going to connect to bluetooth to get the credentials
            //TODO clear this if it comes from the failed setup
            //TODO have this only do the client credentials if the wifi setup was good
            //TODO have this only do the wifi credentials if the boards has already been onboarded
            Serial.println("Getting Credentials");
            if(!hasWifiCredentials()){
                //TODO change this once we have the bluetooth setup
                ssid = getWifiSsid();
                password = getWifiPassword();
            } else {
                ssid = getWifiSsid();
                password = getWifiPassword();
            }

            break;
        }
        case wifiInit: {
            //TODO have this fail after a certain number of tries and reset the credentials
            Serial.println("Initializing Wifi");
            //Initializing the Wifi
            boolean wifiInit = wifi_init(server_name, https);
            if (!wifiInit) {
                break;
            }
            //Waiting for the clock to sync
            waitForSync();

            //Setting up the time to check for firmware updates (also sets up the clock)
            time_t firmwareTime = firmwareUpdateTime();
            time_t heartbeatTime = heartbeatUpdateTime(heartbeatPeriod);

            //Setting up the firmware check event
            Serial.println("Setting up firmware check event and heartbeat event");
            myTimezone.setEvent(firmwareCheck, firmwareTime);
            myTimezone.setEvent(sendHeartbeat, heartbeatTime);

            //Authenticate with the server
            //serverAuth();
            break;
        }
        case Setup: {
            //TODO maybe not have a flash variable for this since the server returns true even if it has already been onboarded
            Serial.println("Setting up");
            setupComplete = isSetupComplete();
            Serial.println("Setup complete: " + String(setupComplete));
            int setupHttpCode = onboardBoard();

            if (setupHttpCode != 200) {
                setupTries++;
                if (setupTries >= setupThreshold) {
                    setupFailed = true;
                }
            } else {
                setupFailed = false;
            }


            digitalWrite(led_1, HIGH);
            delay(500);
            digitalWrite(led_1, LOW);
            delay(500);
            digitalWrite(led_1, HIGH);
            delay(500);
            digitalWrite(led_1, LOW);
            break;

            break;
        }
        case Transmit: {
            Serial.println("Transmitting");
            digitalWrite(led_1, LOW);
            // Checking to make sure the wifi is configured
            if (WiFi.status() != WL_CONNECTED) {
                wifiConnect();
            } else if (!authenticated) {
                //TODO figure out a better way to handle this
                break;
            } else {

                //TODO add a first time send of the current status just to initialize

                //Retrying to send the data if the previous request failed
                if (previousHttpCode != 200) {
                    int httpCode = machineStatusUpdate(machineStatus);
                    //TODO add a way to not have this run every tick after a couple of failed attempts
                    previousHttpCode = httpCode;
                }

                // Transmitting the data if the machine status has changed
                if (machineStatus != previousMachineStatus) {
                    previousMachineStatus = machineStatus;
                    int httpCode = machineStatusUpdate(machineStatus);
                    //TODO figure out what to do if it doesn't send correctly
                    previousHttpCode = httpCode;
                } else {
                    Serial.println("Machine status has not changed");
                }
            }
            break;
        }
        case Authenticate: {
            Serial.println("Authenticating");
            digitalWrite(led_1, LOW);
            if (WiFi.status() != WL_CONNECTED) {
                wifiConnect();
            }
            previousHttpCode = serverAuth();


            break;
        }
        default:
            break;
    }

    // Transitions
    switch (Sensor_State) {
        case Start: {
            if (WiFi.status() != WL_CONNECTED) {
                Sensor_State = getCreds;
            } else if (!setupComplete) {
                Sensor_State = Setup;
                //Serial.println("Next state: Setup");
            } else if (!authenticated) {
                Sensor_State = Authenticate;
                //Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                //Serial.println("Next state: Transmit");
            }
            break;
        }
        case getCreds: {
            //TODO have this fail after a certain number of tries and reset the credentials
            //TODO put bluetooth here (or maybe another state) to get the wifi credentials
            if (hasWifiCredentials()) {
                Sensor_State = wifiInit;
            } else {
                Sensor_State = getCreds;
            }
            break;
        }
        case wifiInit: {
            //TODO have this fail after a certain number of tries and reset the credentials
            if (WiFi.status() == WL_CONNECTED) {
                if (!setupComplete) {
                    Sensor_State = Setup;
                    Serial.println("Next state: Setup");
                } else if (!authenticated) {
                    Sensor_State = Authenticate;
                    //Serial.println("Next state: Authenticate");
                } else {
                    Sensor_State = Transmit;
                    //Serial.println("Next state: Transmit");
                }
            } else {
                Sensor_State = wifiInit;
                //In the future we might want more complex logic here to handle incorrect credentials and other problems
            }
            break;
        }
        case Setup: {
            //TODO have this go back to the getCreds state if it fails enough times
            if (setupFailed) {
                Sensor_State = getCreds;
            } else if (!setupComplete) {
                Sensor_State = Setup;
                Serial.println("Next state: Setup");
            } else { //We can assume that if the board is onboarding then it has not been authenticated yet so it needs to do that first
                Sensor_State = Authenticate;
                Serial.println("Next state: Authenticate");
            }
            break;
        }
        case Transmit: {

            if (WiFi.status() != WL_CONNECTED) {
                Sensor_State = getCreds;
            }

            if (!authenticated) {
                Sensor_State = Authenticate;
                //Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                //Serial.println("Next state: Transmit");
            }
            break;
        }
        case Authenticate: {

            if (WiFi.status() != WL_CONNECTED) {
                Sensor_State = getCreds;
            }

            if (!authenticated) {
                Sensor_State = Authenticate;
                //Serial.println("Next state: Authenticate");
            } else {
                Sensor_State = Transmit;
                //Serial.println("Next state: Transmit");
            }
            break;
        }
        case Wait: {//TODO Will be used for waiting for authentication
            Sensor_State = Transmit;
            break;
        }
        default: {
            Sensor_State = Start;
            break;
        }
    }
}