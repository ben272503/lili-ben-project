#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  uint8_t id;
} struct_message;

struct_message myData;

uint8_t registeredPeers[5][6];  // MACs des esclaves
uint8_t currentID = 1;

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  const uint8_t *mac = recv_info->src_addr;

  Serial.print("Reçu de : ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  if (currentID <= 5) {
    myData.id = currentID;
    esp_now_send(mac, (uint8_t *)&myData, sizeof(myData));
    Serial.printf("Assigné ID %d\n", currentID);
    currentID++;
  } else {
    Serial.println("Nombre maximum d'esclaves atteint");
  }
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Rien ici, tout est géré via la réception
}
