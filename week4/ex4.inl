// A, b, C, d, E, F - active-low
const byte SEG_LETTER[6] = {
  0x88, // A
  0x83, // b
  0xC6, // C
  0xA1, // d
  0x86, // E
  0x8E  // F
};

void setup() {
  setupDisplay();
}

void loop() {
  for (byte i = 0; i < 6; i++) {
    writeDisplay(SEG_LETTER[i], DIGIT_SEL[0], true); // LSBFIRST
    delay(5000);
  }
}