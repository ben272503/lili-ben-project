#include <esp_now.h>
#include <WiFi.h>

#define LED_BUILTIN 8  // LED intégrée ESP32-C3

typedef struct struct_message {
  char text[10];
} struct_message;

struct_message incomingData;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  Serial.print("Message reçu : ");
  Serial.println(incomingData.text);

  if (String(incomingData.text) == "ON") {
    digitalWrite(LED_BUILTIN, LOW);  // LED ON (car LED active LOW)
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH); // LED OFF
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // LED éteinte

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur init ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Récepteur prêt !");
}

void loop() {
  // Pas besoin de code ici, tout se fait en callback
}
