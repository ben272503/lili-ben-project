// Boutons sur 12, 9, 7 — LEDs sur 13, 10, 8

const int bouton1 = 12;
const int led1 = 13;

const int bouton2 = 9;
const int led2 = 10;

const int bouton3 = 7;
const int led3 = 8;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(bouton1, INPUT_PULLUP);
  pinMode(bouton2, INPUT_PULLUP);
  pinMode(bouton3, INPUT_PULLUP);
}

void loop() {
  digitalWrite(led1, digitalRead(bouton1) == HIGH ? HIGH : LOW);
  digitalWrite(led2, digitalRead(bouton2) == HIGH ? HIGH : LOW);
  digitalWrite(led3, digitalRead(bouton3) == HIGH ? HIGH : LOW);
}
