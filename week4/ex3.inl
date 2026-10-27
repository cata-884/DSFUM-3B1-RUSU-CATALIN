Buton b1, b2, b3;
int counter = 0;

void setup() {
  setupDisplay();
  initButon(b1, A1);
  initButon(b2, A2);
  initButon(b3, A3);
}

void loop() {
  if (apasat(b1)) {
    if (counter < 9999) counter++;
  }

  if (apasat(b2)) {
    if (counter > 0) counter--;
  }

  if (apasat(b3)) {
    counter = 0;
  }

  displayNumber(counter, false, false);
}