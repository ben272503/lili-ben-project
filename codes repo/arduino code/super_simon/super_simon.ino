struct Couleur {
  int ledPin;
  int boutonPin;
};

// Mise à jour des pins selon les nouvelles valeurs
Couleur couleurs[3] = {
  {11, 10}, // LED1 ↔ Bouton1
  {9, 8},  // LED2 ↔ Bouton2
  {7, 6}    // LED3 ↔ Bouton3
};

const int maxSequence = 100;
int sequence[maxSequence];
int niveau = 1;  // Démarre à 1 pour commencer avec une séquence d'une LED

void setup() {
  Serial.begin(9600);

  // Initialisation des pins
  for (int i = 0; i < 3; i++) {
    pinMode(couleurs[i].ledPin, OUTPUT);
    pinMode(couleurs[i].boutonPin, INPUT_PULLUP);
  }

  introJeu();
  delay(1000);
  randomSeed(analogRead(0));
  niveau = 1;
  sequence[0] = random(0, 3);  // Commencer avec une seule étape
}

void loop() {
  montrerSequence();
  if (attendreReponse()) {
    delay(500);
    nouvelleEtape();
  } else {
    gameOver();
    niveau = 1;  // Réinitialiser au niveau 1 après une erreur
    delay(2000);
    nouvelleEtape();
  }
}

// === FONCTIONS DU JEU ===

void introJeu() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(couleurs[i].ledPin, HIGH);
    Serial.println("SON_" + String(i));
    delay(300);
    digitalWrite(couleurs[i].ledPin, LOW);
    delay(100);
  }
}

void nouvelleEtape() {
  sequence[niveau] = random(0, 3); // Ajouter une nouvelle LED à la séquence
  niveau++; // Incrémenter le niveau pour ajouter un élément supplémentaire à la séquence
}

void montrerSequence() {
  for (int i = 0; i < niveau; i++) {
    int index = sequence[i];
    digitalWrite(couleurs[index].ledPin, HIGH);
    Serial.println("SON_" + String(index));  // Envoie du message pour jouer un son
    delay(500);
    digitalWrite(couleurs[index].ledPin, LOW);
    delay(200);
  }
}

bool attendreReponse() {
  for (int i = 0; i < niveau; i++) {
    int choix = attendreBouton();
    if (choix != sequence[i]) {
      return false;  // Si un choix est incorrect
    }
  }
  return true;  // Si l'utilisateur a bien suivi la séquence
}

int attendreBouton() {
  while (true) {
    for (int i = 0; i < 3; i++) {
      if (digitalRead(couleurs[i].boutonPin) == LOW) {  // Vérifier si un bouton est pressé
        digitalWrite(couleurs[i].ledPin, HIGH);
        Serial.println("SON_" + String(i));  // Envoie du message pour jouer un son
        delay(300);
        digitalWrite(couleurs[i].ledPin, LOW);

        while (digitalRead(couleurs[i].boutonPin) == LOW); // Attendre que le bouton soit relâché
        delay(100);
        return i;
      }
    }
  }
}

void gameOver() {
  // Allume toutes les LEDs en cas d'erreur
  for (int i = 0; i < 3; i++) {
    digitalWrite(couleurs[i].ledPin, HIGH);
  }
  Serial.println("ERREUR");  // Envoie un message d'erreur
  delay(1000);
  // Éteint toutes les LEDs
  for (int i = 0; i < 3; i++) {
    digitalWrite(couleurs[i].ledPin, LOW);
  }
}
