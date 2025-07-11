void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Démarrage...");
}

void loop() {
  Serial.println("Test");
  delay(1000);
}