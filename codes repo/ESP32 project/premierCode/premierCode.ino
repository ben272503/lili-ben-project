#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_BUILTIN 8  // LED intégrée sur l’ESP32-C3 (souvent GPIO 8)

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"

class CommandCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override {
    String value = pCharacteristic->getValue().c_str();
    Serial.print("Commande reçue : ");
    Serial.println(value.c_str());

    if (value == "ON") {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED allumée");
    } else if (value == "OFF") {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED éteinte");
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  BLEDevice::init("ESP32-C3-BLE");
  BLEServer* pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ
                    );

  pCharacteristic->setCallbacks(new CommandCallback());
  pCharacteristic->setValue("Prêt à recevoir");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("En attente de commandes BLE...");
}

void loop() {
  // Rien à faire ici, on réagit aux commandes BLE
}
