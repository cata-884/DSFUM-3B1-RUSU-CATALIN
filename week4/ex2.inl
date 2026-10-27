const int NR = 10;
unsigned long timpi[NR];
int valori[NR];
int runda = 0;

void setup() {
  setupDisplay();
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop() {
  if (runda >= NR) {
    raport();
    while (1) {}
  }

  int target = random(1000, 10000);

  Serial.print("Afiseaza: ");
  Serial.println(target);

  unsigned long start = millis();
  String input = "";
  bool gata = false;

  while (!gata) {
    displayNumber(target, true, false);

    while (Serial.available()) {
      char c = Serial.read();

      if (c == '\n' || c == '\r') {
        if (input.length() > 0) {
          unsigned long t = millis() - start;

          timpi[runda] = t;
          valori[runda] = target;
          runda++;

          Serial.print("Timp: ");
          Serial.print(t);
          Serial.println(" ms");

          input = "";
          gata = true;
          break;
        }
      } else {
        input += c;
      }
    }
  }

  delay(300);
}

void raport() {
  unsigned long tMin = timpi[0];
  unsigned long tMax = timpi[0];
  unsigned long suma = 0;

  int vMin = valori[0];
  int vMax = valori[0];

  for (int i = 0; i < NR; i++) {
    suma += timpi[i];

    if (timpi[i] < tMin) {
      tMin = timpi[i];
      vMin = valori[i];
    }

    if (timpi[i] > tMax) {
      tMax = timpi[i];
      vMax = valori[i];
    }
  }

  Serial.println("REZULTATE");
  Serial.print("Timp minim: ");
  Serial.print(tMin);
  Serial.print(" ms pentru ");
  Serial.println(vMin);

  Serial.print("Timp maxim: ");
  Serial.print(tMax);
  Serial.print(" ms pentru ");
  Serial.println(vMax);

  Serial.print("Media: ");
  Serial.print((float)suma / NR, 2);
  Serial.println(" ms");
}