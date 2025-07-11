#include <esp_now.h>
#include <WiFi.h>

#define LED_EMETTEUR 4
#define LED_EMETTEUR2 5
#define LED_DE_CHARGE 3
#define BUTTON_PIN 1

#define BUZZER_PIN 7

typedef struct struct_message {
  char text[10];
} struct_message;

struct_message myData;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Dernier message envoyé vers : ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" - Statut : ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Succès" : "Échec");

  // Éteindre la LED émetteur après envoi
  digitalWrite(LED_EMETTEUR, LOW);
  digitalWrite(LED_EMETTEUR2, LOW);
  digitalWrite(LED_DE_CHARGE, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_EMETTEUR, OUTPUT);
  pinMode(LED_EMETTEUR2, OUTPUT);
  pinMode(LED_DE_CHARGE, OUTPUT);
  digitalWrite(LED_EMETTEUR, LOW);
  digitalWrite(LED_EMETTEUR2, LOW);
  digitalWrite(LED_DE_CHARGE, LOW);

  pinMode(BUZZER_PIN, OUTPUT);//initialisation du buzzer

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Bouton avec pull-up interne

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur d'initialisation ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Ajouter le peer broadcast (tout le monde)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(broadcastAddress)) {
    esp_err_t addStatus = esp_now_add_peer(&peerInfo);
    if (addStatus != ESP_OK) {
      Serial.println("Erreur ajout peer");
      return;
    }
  }
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {  // bouton appuyé (actif LOW)
    strcpy(myData.text, "ON");
    digitalWrite(LED_EMETTEUR, HIGH);  // Allumer la LED émetteur
    digitalWrite(LED_EMETTEUR2, HIGH);
    digitalWrite(LED_DE_CHARGE, HIGH);

    delay(50); //histoire de voir les leds s'allumer assez longtemps

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.println("Message envoyé !");
    } else {
      Serial.println("Erreur d'envoi");
      digitalWrite(LED_EMETTEUR, LOW);
      digitalWrite(LED_EMETTEUR2, LOW);
      digitalWrite(LED_DE_CHARGE, LOW);

      noTone(BUZZER_PIN);
    }

    tone(BUZZER_PIN, 1000);  // fréquence en Hz
    delay(50);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, 1000);
    delay(50);
    noTone(BUZZER_PIN);
    // Anti-rebond et éviter plusieurs envois
    delay(500);

    // Attendre que le bouton soit relâché pour pouvoir renvoyer
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(10);
    }
    digitalWrite(LED_EMETTEUR, LOW);  // Éteindre la LED après relâchement
    digitalWrite(LED_EMETTEUR2, LOW);
    digitalWrite(LED_DE_CHARGE, LOW);

    noTone(BUZZER_PIN);
  }
}
