#include <Arduino.h>
#include <ArduinoBLE.h>

#define SERVICE_UUID        "26efde6e-344c-47a4-bf50-78d548220c87"
#define SSID_UUID           "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID       "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID      "592e1d83-70a4-4c3e-8917-dcd2c63985fb"
#define CLIENTNAME_UUID     "27ea72f0-6f5a-4274-9220-80dcb1778570"

BLEService wifiService(SERVICE_UUID);
BLECharacteristic ssidCharacteristic(SSID_UUID, BLEWrite, 32);
BLECharacteristic passwordCharacteristic(PASSWORD_UUID, BLEWrite, 32);
BLECharacteristic clientKeyCharacteristic(CLIENTKEY_UUID, BLEWrite, 32);
BLECharacteristic clientNameCharacteristic(CLIENTNAME_UUID, BLEWrite, 32);

String ssid = "DefaultSSID";
String password = "DefaultPassword";
String clientKey = "DefaultClientKey";
String clientName = "DefaultClientName";

void onSSIDWritten(BLEDevice central, BLECharacteristic characteristic);
void onPasswordWritten(BLEDevice central, BLECharacteristic characteristic);
void onClientKeyWritten(BLEDevice central, BLECharacteristic characteristic);
void onClientNameWritten(BLEDevice central, BLECharacteristic characteristic);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }

    BLE.setLocalName("Washr");
    BLE.setAdvertisedService(wifiService);

    wifiService.addCharacteristic(ssidCharacteristic);
    wifiService.addCharacteristic(passwordCharacteristic);
    wifiService.addCharacteristic(clientKeyCharacteristic);
    wifiService.addCharacteristic(clientNameCharacteristic);

    BLE.addService(wifiService);

    ssidCharacteristic.writeValue(ssid.c_str());
    passwordCharacteristic.writeValue(password.c_str());
    clientKeyCharacteristic.writeValue(clientKey.c_str());
    clientNameCharacteristic.writeValue(clientName.c_str());

    ssidCharacteristic.setEventHandler(BLEWritten, onSSIDWritten);
    passwordCharacteristic.setEventHandler(BLEWritten, onPasswordWritten);
    clientKeyCharacteristic.setEventHandler(BLEWritten, onClientKeyWritten);
    clientNameCharacteristic.setEventHandler(BLEWritten, onClientNameWritten);

    BLE.advertise();

    Serial.println("BLE Peripheral - Advertising");
}

void loop() {
    BLEDevice central = BLE.central();

    Serial.println("Waiting to connect");

    if (central.connected()) {
        Serial.print("Connected to central: ");
        Serial.println(String(central.address()));

        while (central.connected()) {
            // Do nothing, just wait for the central to disconnect
        }

        Serial.print("Disconnected from central: ");
        Serial.println(String(central.address()));
    }
}

void onSSIDWritten(BLEDevice central, BLECharacteristic characteristic) {
    char buffer[32];
    int length = characteristic.readValue(buffer, sizeof(buffer) - 1);
    buffer[length] = '\0'; // Ensure null-termination
    ssid = String(buffer);
    Serial.print("SSID updated to: ");
    Serial.println(ssid);
}

void onPasswordWritten(BLEDevice central, BLECharacteristic characteristic) {
    char buffer[32];
    int length = characteristic.readValue(buffer, sizeof(buffer) - 1);
    buffer[length] = '\0'; // Ensure null-termination
    password = String(buffer);
    Serial.print("Password updated to: ");
    Serial.println(password);
}

void onClientKeyWritten(BLEDevice central, BLECharacteristic characteristic) {
    char buffer[32];
    int length = characteristic.readValue(buffer, sizeof(buffer) - 1);
    buffer[length] = '\0'; // Ensure null-termination
    clientKey = String(buffer);
    Serial.print("Client Key updated to: ");
    Serial.println(clientKey);
}

void onClientNameWritten(BLEDevice central, BLECharacteristic characteristic) {
    char buffer[32];
    int length = characteristic.readValue(buffer, sizeof(buffer) - 1);
    buffer[length] = '\0'; // Ensure null-termination
    clientName = String(buffer);
    Serial.print("Client Name updated to: ");
    Serial.println(clientName);
}