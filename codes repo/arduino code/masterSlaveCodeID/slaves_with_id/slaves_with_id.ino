#include <WiFi.h>
#include <esp_now.h>

#define LED_PIN 8  // LED interne pour ESP32 (GPIO 2)

typedef struct struct_message {
  uint8_t id;
} struct_message;

struct_message myData;

uint8_t masterAddress[] = {0x34, 0x85, 0x18, 0xC5, 0x3A, 0xA8};; // À modifier selon l'adresse MAC du maître
bool idReceived = false;

void OnDataRecv(const esp_now_recv_info_t * recv_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("ID reçu : %d\n", myData.id);
  idReceived = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, masterAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(masterAddress)) {
    esp_now_add_peer(&peerInfo);
  }

  // Envoi d'une demande au maître
  esp_now_send(masterAddress, NULL, 0);
}

void loop() {
  if (idReceived) {
    delay(myData.id * 2000);  // Délai basé sur l'ID
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    idReceived = false;  // pour ne pas répéter
  }
}
