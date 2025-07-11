#define LED1 4  // Broche LED 1
#define LED2 5  // Broche LED 2
#define BUTTON_PIN 1  // Broche du bouton
#define BUZZER_PIN 7  // Broche du buzzer

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Activation du pull-up interne pour le bouton

  Serial.begin(115200);  // Optionnel pour le debug
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {  // Bouton pressé
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    
    // Émettre un son sur le buzzer avec PWM (fréquence 1000 Hz)
    tone(BUZZER_PIN, 1000);  // fréquence en Hz (ex: 1000 Hz)

  } else {  // Bouton relâché
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);

    noTone(BUZZER_PIN);
  }
  delay(10);  // Anti-rebonds léger
}

