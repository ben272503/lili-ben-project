#include <BLEDevice.h>
#include <BLEScan.h>

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  pBLEScan->start(0, nullptr);  // Scan infini
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    std::string advName = advertisedDevice.getName();
    Serial.print("Pub détectée: ");
    Serial.println(advName.c_str());

    // Ici tu peux récupérer des données dans les données d'advertising (ex: manufacturer data)
    // Puis allumer ou éteindre ta LED selon contenu
  }
};
