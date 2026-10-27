void setup() {
  setupDisplay();
}

void loop() {
  for (byte d = 0; d <= 9; d++) {
    writeDisplay(SEG_DIGIT[d], DIGIT_SEL[0], true);
    delay(1000);
  }
}