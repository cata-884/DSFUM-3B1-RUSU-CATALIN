char text[] = "dECEbAL_Si_FiUL_SAU_LOCUiESC_PE_PLAiUL_dACiEi";
const int textLen = sizeof(text) - 1;

Buton b1, b2, b3;

bool paused = false;
int offset = -3;
unsigned long lastScroll = 0;
const unsigned long SCROLL_MS = 350;

byte charToSeg(char c) {
  switch (c) {
    case '0': return SEG_DIGIT[0];
    case '1': return SEG_DIGIT[1];
    case '2': return SEG_DIGIT[2];
    case '3': return SEG_DIGIT[3];
    case '4': return SEG_DIGIT[4];
    case '5': return SEG_DIGIT[5];
    case '6': return SEG_DIGIT[6];
    case '7': return SEG_DIGIT[7];
    case '8': return SEG_DIGIT[8];
    case '9': return SEG_DIGIT[9];

    case 'A': return 0x88;
    case 'b': return 0x83;
    case 'C': return 0xC6;
    case 'd': return 0xA1;
    case 'E': return 0x86;
    case 'F': return 0x8E;

    case '_': return 0xF7; // segmentul de jos
    case 'L': return 0xC7;
    case 'i': return 0xFB; // o reprezentare pentru i mic
    case 'J': return 0xE1;
    case 'O': return 0xC0; // ca 0
    case 'P': return 0x8C;
    case 'S': return 0x92; // ca 5
    case 'U': return 0xC3;

    case ' ': return SEG_BLANK;
    default:  return SEG_BLANK;
  }
}

void displayWindow(int off) {
  for (int pos = 0; pos < 4; pos++) {
    int idx = off + pos;
    byte seg = SEG_BLANK;

    if (idx >= 0 && idx < textLen) {
      seg = charToSeg(text[idx]);
    }

    writeDisplay(seg, DIGIT_SEL[pos], false);
    delay(2);
  }
}

void setup() {
  setupDisplay();

  initButon(b1, A1);
  initButon(b2, A2);
  initButon(b3, A3);

  lastScroll = millis();
}

void loop() {
  // citim toate butoanele
  bool p1 = apasat(b1);
  bool p2 = apasat(b2);
  bool p3 = apasat(b3);

  if (p1 || p2 || p3) {
    paused = !paused;
  }

  if (!paused && millis() - lastScroll >= SCROLL_MS) {
    lastScroll = millis();
    offset++;

    if (offset > textLen) {
      offset = -3;
    }
  }

  displayWindow(offset);
}