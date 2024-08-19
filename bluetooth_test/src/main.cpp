#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "26efde6e-344c-47a4-bf50-78d548220c87"
#define CHARACTERISTIC_UUID "0f066506-bbe6-43cf-8697-4b5ba6b1cc6d"
#define SSID_UUID "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID "592e1d83-70a4-4c3e-8917-dcd2c63985fb"

class WiFiCredentials {
public:
    String ssid;
    String password;
    String key;
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String value = String(pCharacteristic->getValue().c_str());

        if (value.length() > 0) {
            Serial.println("*********");
            Serial.print("New value: ");
            for (int i = 0; i < value.length(); i++)
                Serial.print(value[i]);

            Serial.println();
            Serial.println("*********");
        }
    }
};

class WiFiCallbacks: public BLECharacteristicCallbacks {
private:
    WiFiCredentials *credentials;
    String uuid;

public:
    WiFiCallbacks(WiFiCredentials *creds, String uuid) : credentials(creds), uuid(uuid) {}

    void onWrite(BLECharacteristic *pCharacteristic) override {
        String value = String(pCharacteristic->getValue().c_str());

        if (value.length() > 0) {
            Serial.print("New value for ");
            Serial.print(uuid);
            Serial.print(": ");
            Serial.println(value);

            if (uuid == SSID_UUID) {
                credentials->ssid = value;
                Serial.println("SSID: " + credentials->ssid);
            } else if (uuid == PASSWORD_UUID) {
                credentials->password = value;
                Serial.println("Password: " + credentials->password);
            } else if (uuid == CLIENTKEY_UUID) {
                credentials->key = value;
                Serial.println("Client Key: " + credentials->key);
            }
        }
    }
};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        Serial.println("Client connected");
    };

    void onDisconnect(BLEServer *pServer) {
        Serial.println("Client disconnected - restarting advertising");
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->start();
    }
};

void setup() {
    Serial.begin(115200);

    WiFiCredentials credentials;

    BLEDevice::init("MyESP32");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks());

    pCharacteristic->setValue("Hello World");

    BLECharacteristic *ssidCharacteristic = pService->createCharacteristic(
            SSID_UUID,
            BLECharacteristic::PROPERTY_WRITE
    );
    ssidCharacteristic->setCallbacks(new WiFiCallbacks(&credentials, SSID_UUID));

    BLECharacteristic *passwordCharacteristic = pService->createCharacteristic(
            PASSWORD_UUID,
            BLECharacteristic::PROPERTY_WRITE
    );
    passwordCharacteristic->setCallbacks(new WiFiCallbacks(&credentials, PASSWORD_UUID));

    BLECharacteristic *keyCharacteristic = pService->createCharacteristic(
            CLIENTKEY_UUID,
            BLECharacteristic::PROPERTY_WRITE
    );
    keyCharacteristic->setCallbacks(new WiFiCallbacks(&credentials, CLIENTKEY_UUID));

    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(2000);
}