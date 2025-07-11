const int boutonPin = 12;  // Pin numérique connectée au bouton
const int ledPin = 13;     // Pin numérique connectée à la LED

int longueurSequence = 3;  // Longueur initiale de la séquence
int indexUtilisateur = 0;  // Index pour suivre la position dans la séquence

void setup() {
  pinMode(boutonPin, INPUT);  // Déclare la pin du bouton comme entrée
  pinMode(ledPin, OUTPUT);    // Déclare la pin de la LED comme sortie
  randomSeed(analogRead(0)); // Initialise la génération de nombres aléatoires
  Serial.begin(9600);        // Initialiser la communication série avec l'ordinateur
}

void loop() {
  // Montre 3 bips rapides sur la LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);  // LED allumée
    delay(200);                  // Temps allumé
    digitalWrite(ledPin, LOW);   // LED éteinte
    delay(200);                  // Temps éteint
  }

  // Maintenant le jeu commence avec la séquence de bips
  for (int i = 0; i < longueurSequence; i++) {
    // Le jeu affiche le nombre de bips correspondant à la longueur de la séquence
    for (int j = 0; j < i + 1; j++) {
      digitalWrite(ledPin, HIGH);  // LED allumée
      delay(500);                  // LED allumée pendant 500ms
      digitalWrite(ledPin, LOW);   // LED éteinte
      delay(500);                  // LED éteinte pendant 500ms
    }

    // L'utilisateur doit reproduire le nombre de bips
    int bipsUtilisateur = 0;
    while (bipsUtilisateur < i + 1) {
      int etatBouton = digitalRead(boutonPin);  // Lit l'état du bouton
      if (etatBouton == HIGH) {  // Si le bouton est pressé
        bipsUtilisateur++;      // Incrémente le nombre de bips détectés
        delay(500);             // Petite pause pour éviter les rebonds du bouton
        Serial.println("BIP");  // Envoie un signal "BIP" à l'ordinateur
      }
    }

    // Si l'utilisateur a pressé le bon nombre de bips
    if (bipsUtilisateur == i + 1) {
      delay(500); // Petite pause avant le prochain tour
    } else {
      // Si l'utilisateur se trompe, recommence avec 3 bips
      restartGame();
      return;
    }
  }

  // Si l'utilisateur a bien reproduit toute la séquence, on allonge la séquence
  longueurSequence++;
  delay(2000); // Attend 2 secondes avant de recommencer
}

// Fonction pour redémarrer le jeu en cas d'erreur
void restartGame() {
  longueurSequence = 3;
  indexUtilisateur = 0;
  delay(1000); // Pause avant de recommencer
}
