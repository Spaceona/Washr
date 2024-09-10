#include <Arduino.h>
#include <ArduinoBLE.h>

#define SERVICE_UUID        "26efde6e-344c-47a4-bf50-78d548220c87"
#define SSID_UUID           "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID       "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID      "592e1d83-70a4-4c3e-8917-dcd2c63985fb"
#define CLIENTNAME_UUID     "27ea72f0-6f5a-4274-9220-80dcb1778570"

String ssid = "ESPDevNetwork";
String password = "password";
String clientKey = "testkey";
String clientName = "test2";

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }

    Serial.println("BLE Central - Scanning for peripherals...");

    BLE.scan();
}

void loop() {
    BLEDevice peripheral = BLE.available();

    if (peripheral && peripheral.hasService(SERVICE_UUID)) {
        Serial.print("Found peripheral with service UUID: ");
        Serial.println(SERVICE_UUID);

        if (peripheral.localName() == "Washr") {
            BLE.stopScan();

            if (peripheral.connect()) {
                Serial.println("Connected to peripheral");

                if (peripheral.discoverAttributes()) {
                    Serial.println("Attributes discovered");

                    BLECharacteristic ssidCharacteristic = peripheral.characteristic(SSID_UUID);
                    BLECharacteristic passwordCharacteristic = peripheral.characteristic(PASSWORD_UUID);
                    BLECharacteristic clientKeyCharacteristic = peripheral.characteristic(CLIENTKEY_UUID);
                    BLECharacteristic clientNameCharacteristic = peripheral.characteristic(CLIENTNAME_UUID);

                    if (ssidCharacteristic.canWrite()) {
                        ssidCharacteristic.writeValue(ssid.c_str());
                        Serial.println("SSID written");
                    }

                    if (passwordCharacteristic.canWrite()) {
                        passwordCharacteristic.writeValue(password.c_str());
                        Serial.println("Password written");
                    }

                    if (clientKeyCharacteristic.canWrite()) {
                        clientKeyCharacteristic.writeValue(clientKey.c_str());
                        Serial.println("Client Key written");
                    }

                    if (clientNameCharacteristic.canWrite()) {
                        clientNameCharacteristic.writeValue(clientName.c_str());
                        Serial.println("Client Name written");
                    }
                }

                peripheral.disconnect();
                Serial.println("Disconnected from peripheral");
            } else {
                Serial.println("Failed to connect to peripheral");
            }

            BLE.scan();
        }
    }
}

