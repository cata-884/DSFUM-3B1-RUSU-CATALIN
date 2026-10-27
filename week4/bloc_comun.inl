const uint8_t LATCH = 4;
const uint8_t CLK   = 7;
const uint8_t DATA  = 8;

const uint8_t DIGIT_SEL[4] = {241, 242, 244, 248};

const byte SEG_BLANK = 0xFF;

const byte SEG_DIGIT[10] = {
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0x80, // 8
  0x90  // 9
};

byte rev8(byte b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

void setupDisplay() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  digitalWrite(LATCH, LOW);
}

void writeDisplay(byte seg, byte sel, bool lsbFirst = false) {
  digitalWrite(LATCH, LOW);

  if (lsbFirst) {
    shiftOut(DATA, CLK, LSBFIRST, rev8(seg));
    shiftOut(DATA, CLK, LSBFIRST, rev8(sel));
  } else {
    shiftOut(DATA, CLK, MSBFIRST, seg);
    shiftOut(DATA, CLK, MSBFIRST, sel);
  }

  digitalWrite(LATCH, HIGH);
}

void displayNumber(int value, bool leadingZeros, bool lsbFirst = false) {
  const int p10[4] = {1000, 100, 10, 1};

  int start = 3;
  if (leadingZeros) {
    start = 0;
  } else if (value >= 1000) {
    start = 0;
  } else if (value >= 100) {
    start = 1;
  } else if (value >= 10) {
    start = 2;
  }

  for (int pos = 0; pos < 4; pos++) {
    byte seg = SEG_BLANK;

    if (pos >= start) {
      int digit = (value / p10[pos]) % 10;
      seg = SEG_DIGIT[digit];
    }

    writeDisplay(seg, DIGIT_SEL[pos], lsbFirst);
    delay(2);
  }
}

struct Buton {
  uint8_t pin;
  bool lastRead;
  bool stable;
  unsigned long lastChange;
};

void initButon(Buton &b, uint8_t pin) {
  b.pin = pin;
  pinMode(pin, INPUT_PULLUP);
  b.lastRead = digitalRead(pin);
  b.stable = b.lastRead;
  b.lastChange = millis();
}

bool apasat(Buton &b) {
  bool r = digitalRead(b.pin);

  if (r != b.lastRead) {
    b.lastRead = r;
    b.lastChange = millis();
  }

  if (millis() - b.lastChange > 30 && r != b.stable) {
    b.stable = r;
    if (b.stable == LOW) {
      return true;
    }
  }

  return false;
}