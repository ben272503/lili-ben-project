// Boutons sur 12, 9, 7 — LEDs sur 13, 10, 8

const int bouton1 = 10; //led jaune
const int led1 = 11;

const int bouton2 = 8; //led rouge
const int led2 = 9;

const int bouton3 = 6; //led bleue
const int led3 = 7;

const int maxSteps = 10;         // Taille max du tableau
int sequence[maxSteps];           // Tableau pour stocker la séquence
int currentLength = 0;            // Taille actuelle de la séquence

bool gamestate = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialisation des pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(bouton1, INPUT_PULLUP);
  pinMode(bouton2, INPUT_PULLUP);
  pinMode(bouton3, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  randomSeed(analogRead(A0));
  introJeu();//séquence de présentation du jeu. guirlande

  //pour le programme python: serial println qui me serviront à voir ou on en est
  int numTour = 1;
  while (gamestate == true) {
    Serial.println("--- Tour " + String(numTour) + " ---");//debug python
    sequenceJeu();//un tour en jeu
    numTour++;
  }
  
  //fin du jeu quelque soit issue, attente pour redémarrer
  attente_redemarrage();
}

// === FONCTIONS DU JEU ===

void introJeu() { //sequence d'allumage pour les leds et son de démarrage
  digitalWrite(led1, HIGH);
  Serial.println("DEBUT");
  delay(200);
  digitalWrite(led1, LOW);
  delay(100);
  
  digitalWrite(led2, HIGH);
  delay(200);
  digitalWrite(led2, LOW);
  delay(100);

  digitalWrite(led3, HIGH);
  delay(200);
  digitalWrite(led3, LOW);
  delay(100);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  delay(200);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(200);
}

void sequenceJeu() {
  delay(1000);

  if (currentLength < maxSteps) {
    // Ajouter un nombre aléatoire entre 1 et 3 dans le tableau
    int randomNumber = random(1, 4); // random(1, 4) génère 1, 2 ou 3
    sequence[currentLength] = randomNumber;
    currentLength++;

    // Parcourir et afficher tous les éléments de la séquence
    Serial.println("Séquence a reproduire :");
    for (int i = 0; i < currentLength; i++) {
      
      // Action spécifique pour chaque valeur
      switch (sequence[i]) {
        case 1:
          btn1();
          delay(300);
          btn1_off();
          break;
        case 2:
          btn2();
          delay(300);
          btn2_off();
          break;
        case 3:
          btn3();
          delay(300);
          btn3_off();
          break;
        default:
          break;
      }
      delay(500); // pause entre chaque affichage/LED
    }

    //input du joueur
    Serial.println("À toi de jouer !");
    for (int i = 0; i < currentLength; i++) {
      int expected = sequence[i];
      bool waitforplayer = true;
      while (waitforplayer) {
        // Check for correct button press based on expected value
        if (expected == 1 && digitalRead(bouton1) == HIGH) {
          btn1();
          waitforplayer = false;
        } 
        else if (expected == 2 && digitalRead(bouton2) == HIGH) {
          btn2();
          waitforplayer = false;
       } 
        else if (expected == 3 && digitalRead(bouton3) == HIGH) {
          btn3();
          waitforplayer = false;
        }
        // If any wrong button is pressed
        else if (digitalRead(bouton1) == HIGH && expected != 1 || digitalRead(bouton2) == HIGH && expected != 2 || digitalRead(bouton3) == HIGH && expected != 3) {
          if(digitalRead(bouton1) == HIGH) {
            btn1();
          }
          else if(digitalRead(bouton2) == HIGH) {
            btn2();
          }
          else if(digitalRead(bouton3) == HIGH) {
            btn3();
          }
          erreur();
          i = currentLength;  // To exit the loop early
          waitforplayer = false;
        }
      }
      //__________________________________le probleme est ici
      // Attendre relâchement des boutons avant de recommencer
      while (digitalRead(bouton1) == HIGH ||
         digitalRead(bouton2) == HIGH ||
         digitalRead(bouton3) == HIGH) {
        // Attente que tous les boutons soient relâchés
      }
      btn1_off();
      btn2_off();
      btn3_off();
    }
  }else {//victoire
    if (gamestate == true) {
      Serial.println("LEVEL");
      gamestate = false; //on sort du while.
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      delay(200);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      delay(200);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      delay(200);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      delay(2000);
    }
  }
}

void erreur() {
  delay(200);
  Serial.println("ERREUR");
  btn1_off();
  btn2_off();
  btn3_off();
  delay(200);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  delay(2000);
  btn1_off();
  btn2_off();
  btn3_off();
  gamestate = false; //on sort du while.
  
  //debug python
  Serial.println("___En attente de redémarrage___");
}

void attente_redemarrage() {
  while (digitalRead(bouton2) == LOW || digitalRead(bouton3) == LOW) {  // Tant que le bouton n'est pas pressé (pin est HIGH)
    // Le bouton n'est pas pressé, attendre...
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(400);
  //réinitialisation des variables:
  gamestate = true;
  currentLength = 0;
  sequence[maxSteps] = {0};//remet tout à zéro
}

// ==== Codage des boutons + son + led pour facilité =======
// modifier pour adapter à l'ecoute
void btn1() {
  digitalWrite(led1, HIGH);
  Serial.println("BIP1");
}
void btn2() {
  digitalWrite(led2, HIGH);
  Serial.println("BIP2");
}
void btn3() {
  digitalWrite(led3, HIGH);
  Serial.println("BIP3");
}
void btn1_off() {
  digitalWrite(led1, LOW);
}
void btn2_off() {
  digitalWrite(led2, LOW);
}
void btn3_off() {
  digitalWrite(led3, LOW);
}
