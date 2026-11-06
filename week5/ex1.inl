
const int p_tip = 4;

struct Nota {
    int frecventa;
    int durata;
    int pauza;
};

struct Melodie {
    const Nota* note;
    int nr_note;
};

const Nota melodie0[] = {
    {523, 250, 50},
    {659, 250, 50},
    {784, 400, 100},
    {659, 250, 50},
    {784, 500, 100},
    {0, 200, 50}
};

const Nota melodie1[] = {
    {784, 250, 50},
    {698, 250, 50},
    {659, 250, 50},
    {587, 400, 100},
    {523, 500, 100},
    {0, 200, 50}
};

Melodie melodii[2] = {
    {melodie0, sizeof(melodie0) / sizeof(Nota)},
    {melodie1, sizeof(melodie1) / sizeof(Nota)}
};

char s[100];
int idx = 0;

enum ModAfisare {
    MOD_CLOCK,
    MOD_STOPWATCH
};

ModAfisare modAfisare = MOD_CLOCK;

bool clockReady = false;
bool stopwatchReady = false;
bool stopwatchRunning = false;
bool stopwatchFinished = false;

long clockSeconds = 0;
long stopwatchSeconds = 0;

unsigned long lastClockUpdate = 0;
unsigned long lastStopwatchUpdate = 0;

int clockAlarmHour = -1;
int clockAlarmMinute = -1;
int clockAlarmMelody = -1;

int stopwatchAlarmMelody = 0;

bool melodyPlaying = false;
int melodyIndex = 0;
int melodyNoteIndex = 0;
int melodyPhase = 0;
unsigned long lastMelodyUpdate = 0;

void startMelody(int index) {
    if (index < 0 || index > 1) return;

    melodyIndex = index;
    melodyNoteIndex = 0;
    melodyPhase = 0;
    melodyPlaying = true;

    lastMelodyUpdate = millis();

    int frecventa = melodii[melodyIndex].note[0].frecventa;

    if (frecventa > 0) {
        tone(p_tip, frecventa);
    }
    else {
        noTone(p_tip);
    }
}

void nextMelodyNote() {
    melodyNoteIndex++;

    if (melodyNoteIndex >= melodii[melodyIndex].nr_note) {
        melodyPlaying = false;
        noTone(p_tip);
        return;
    }

    melodyPhase = 0;
    lastMelodyUpdate = millis();

    int frecventa = melodii[melodyIndex].note[melodyNoteIndex].frecventa;

    if (frecventa > 0) {
        tone(p_tip, frecventa);
    }
    else {
        noTone(p_tip);
    }
}

void updateMelody() {
    if (!melodyPlaying) return;

    unsigned long now = millis();

    Nota nota = melodii[melodyIndex].note[melodyNoteIndex];

    if (melodyPhase == 0) {
        if (now - lastMelodyUpdate >= (unsigned long)nota.durata) {
            noTone(p_tip);

            melodyPhase = 1;
            lastMelodyUpdate = now;

            if (nota.pauza == 0) {
                nextMelodyNote();
            }
        }
    }
    else {
        if (now - lastMelodyUpdate >= (unsigned long)nota.pauza) {
            nextMelodyNote();
        }
    }
}

int getClockHour() {
    return (clockSeconds / 3600) % 24;
}

int getClockMinute() {
    return (clockSeconds / 60) % 60;
}

void checkClockAlarm() {
    if (clockAlarmMelody == -1) return;

    int hour = getClockHour();
    int minute = getClockMinute();

    if (clockSeconds % 60 == 0 &&
        hour == clockAlarmHour &&
        minute == clockAlarmMinute) {

        startMelody(clockAlarmMelody);
        }
}

void updateTimers() {
    unsigned long now = millis();

    if (clockReady) {
        unsigned long elapsed = (now - lastClockUpdate) / 1000;

        if (elapsed > 0) {
            lastClockUpdate += elapsed * 1000;

            for (unsigned long i = 0; i < elapsed; i++) {
                clockSeconds++;
                if (clockSeconds >= 86400) {
                    clockSeconds = 0;
                }

                checkClockAlarm();
            }
        }
    }

    if (stopwatchReady && stopwatchRunning) {
        unsigned long elapsed = (now - lastStopwatchUpdate) / 1000;

        if (elapsed > 0) {
            lastStopwatchUpdate += elapsed * 1000;

            if (elapsed >= (unsigned long)stopwatchSeconds) {
                stopwatchSeconds = 0;
                stopwatchRunning = false;

                if (!stopwatchFinished) {
                    stopwatchFinished = true;
                    startMelody(stopwatchAlarmMelody);
                }
            }
            else {
                stopwatchSeconds -= elapsed;
            }
        }
    }
}

void updateDisplay() {
    int valoare;

    if (modAfisare == MOD_CLOCK) {
        int hour = getClockHour();
        int minute = getClockMinute();

        valoare = hour * 100 + minute;
    }
    else {
        int minute = stopwatchSeconds / 60;
        int secunde = stopwatchSeconds % 60;

        valoare = minute * 100 + secunde;
    }

    displayNumber(valoare, false, false);
}

bool validTime(int hour, int minute) {
    return hour >= 0 && hour < 24 &&
    minute >= 0 && minute < 60;
}

void process_finished_stream() {
    int hour, minute, melody;

    if (sscanf(s, "clock %d:%d", &hour, &minute) == 2) {
        if (!clockReady) {
            if (validTime(hour, minute)) {
                clockSeconds = hour * 3600L + minute * 60L;
                lastClockUpdate = millis();
                clockReady = true;
                modAfisare = MOD_CLOCK;

                Serial.println("Clock setat");
            }
            else {
                Serial.println("Ora invalida");
            }
        }
        else {
            Serial.println("Clock a fost deja setat");
        }
    }

    else if (sscanf(s, "stopwatch %d:%d", &minute, &hour) == 2) {
        if (!stopwatchReady) {
            if (minute >= 0 && minute <= 99 &&
                hour >= 0 && hour < 60) {

                stopwatchSeconds = minute * 60L + hour;
            lastStopwatchUpdate = millis();

            stopwatchReady = true;
            stopwatchRunning = stopwatchSeconds > 0;
            stopwatchFinished = false;

            modAfisare = MOD_STOPWATCH;

            if (stopwatchSeconds == 0) {
                stopwatchFinished = true;
                startMelody(stopwatchAlarmMelody);
            }

            Serial.println("Stopwatch setat");
                }
                else {
                    Serial.println("Timp invalid");
                }
        }
        else {
            Serial.println("Stopwatch a fost deja setat");
        }
    }

    else if (sscanf(s, "alarm clock %d:%d %d",
        &hour, &minute, &melody) == 3) {

        if (validTime(hour, minute) &&
            melody >= 0 && melody <= 1) {

            clockAlarmHour = hour;
        clockAlarmMinute = minute;
    clockAlarmMelody = melody;

    Serial.println("Alarma clock setata");
            }
            else {
                Serial.println("Parametri alarma invalizi");
            }
        }

        else if (sscanf(s, "alarm stopwatch %d", &melody) == 1) {
            if (melody >= 0 && melody <= 1) {
                stopwatchAlarmMelody = melody;

                Serial.println("Alarma stopwatch setata");
            }
            else {
                Serial.println("Index melodie invalid");
            }
        }

        else if (strcmp(s, "clock") == 0) {
            if (clockReady) {
                modAfisare = MOD_CLOCK;
                Serial.println("Mod clock");
            }
            else {
                Serial.println("Clock nu a fost initializat");
            }
        }

        else if (strcmp(s, "stopwatch") == 0) {
            if (stopwatchReady) {
                modAfisare = MOD_STOPWATCH;
                Serial.println("Mod stopwatch");
            }
            else {
                Serial.println("Stopwatch nu a fost initializat");
            }
        }

        else {
            Serial.println("Comanda invalida");
        }

        memset(s, 0, sizeof(s));
        idx = 0;
}

void setup() {
    setupDisplay();
    Serial.begin(9600);

    pinMode(p_tip, OUTPUT);
    noTone(p_tip);

    Serial.println("Introduceti o comanda:");
}

void loop() {
    char c;

    if (Serial.available() > 0) {
        c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (idx > 0) {
                s[idx] = '\0';
                process_finished_stream();
            }
        }
        else {
            if (idx < (int)sizeof(s) - 1) {
                s[idx++] = c;
            }
            else {
                idx = 0;
                memset(s, 0, sizeof(s));
                Serial.println("Comanda prea lunga");
            }
        }
    }

    updateTimers();
    updateMelody();
    updateDisplay();
}
