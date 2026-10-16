enum StareLED {
    STINS,
    APRINS,
    FADE_IN,
    FADE_OUT,
    FADE_IN_OUT
};


struct Input{
    LED,
    STATE,
    MILIS
}

char s[100];
int idx = 0;

Input input;
const int NUM_LEDS = 6;
const int pins[NUM_LEDS] = {8, 9, 10, 11, 12, 13};

StareLED stare[NUM_LEDS];
int brightness[NUM_LEDS];
int fadeDirection[NUM_LEDS];
int fadeStep[NUM_LEDS];
unsigned long lastFade[NUM_LEDS];
unsigned long startTime[NUM_LEDS];
unsigned long duration[NUM_LEDS];

const unsigned long PWM_PERIOD_US = 1000;

void set_stare(int led, StareLED s, unsigned long durata_ms) {
    stare[led] = s;
    duration[led] = durata_ms;
    startTime[led] = millis();
    lastFade[led] = millis();

    switch (s) {
        case STINS:
            brightness[led] = 0;
            break;
        case APRINS:
            brightness[led] = 255;
            break;
        case FADE_IN:
            brightness[led] = 0;
            fadeDirection[led] = 1;
            break;
        case FADE_OUT:
            brightness[led] = 255;
            fadeDirection[led] = -1;
            break;
        case FADE_IN_OUT:
            brightness[led] = 0;
            fadeDirection[led] = 1;
            break;
    }
}

void update_led(int led) {
    unsigned long now = millis();
    unsigned long elapsed = now - startTime[led];

    switch (stare[led]) {
        case STINS:
            brightness[led] = 0;
            break;

        case APRINS:
            brightness[led] = 255;
            break;

        case FADE_IN:
            if (now - lastFade[led] >= 20) {
                lastFade[led] = now;
                brightness[led] += fadeStep[led];
                if (brightness[led] >= 255) brightness[led] = 255;
            }
            if (duration[led] > 0 && elapsed >= duration[led]) {
                set_stare(led, APRINS, 0);
            }
            break;

        case FADE_OUT:
            if (now - lastFade[led] >= 20) {
                lastFade[led] = now;
                brightness[led] -= fadeStep[led];
                if (brightness[led] <= 0) brightness[led] = 0;
            }
            if (duration[led] > 0 && elapsed >= duration[led]) {
                set_stare(led, STINS, 0);
            }
            break;

        case FADE_IN_OUT:
            if (now - lastFade[led] >= 40) {
                lastFade[led] = now;
                brightness[led] += fadeDirection[led] * 2;
                if (brightness[led] >= 255) {
                    brightness[led] = 255;
                    fadeDirection[led] = -1;
                } else if (brightness[led] <= 0) {
                    brightness[led] = 0;
                    fadeDirection[led] = 1;
                }
            }
            break;
    }
}

//necesar pentru led8,13
void software_pwm(int led, unsigned long nowUs) {
    unsigned long duty = (unsigned long)((brightness[led] / 255.0) * PWM_PERIOD_US);
    if ((nowUs % PWM_PERIOD_US) < duty)
        digitalWrite(pins[led], HIGH);
    else
        digitalWrite(pins[led], LOW);
}

void setup() {
    Serial.begin(9600);

    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(pins[i], OUTPUT);
        fadeStep[i] = 5;
        set_stare(i, STINS, 0);
    }

    set_stare(0, FADE_IN_OUT, 0);
    set_stare(2, FADE_IN, 2000);
    set_stare(3, APRINS, 0);
    set_stare(4, FADE_OUT, 2000);
    set_stare(5, STINS, 0);
}

/*
 v *oid populare_obiect(Input& input) {
 int s_it = 0, nr_it = 0, nr_cnt = 0;
 char nr[20] = {0};
 while (s[s_it] != '\n' && s[s_it] != '\0') {
     if (is_op(s[s_it])) {
         if (nr_it > 0) {
             nr[nr_it] = '\0';
             calc.n[nr_cnt++] = atoi(nr);
             nr_it = 0;
             }
             calc.op = s[s_it];
             }
             else if (s[s_it] == ' ') {
                 if (nr_it > 0) {
                     nr[nr_it] = '\0';
                     calc.n[nr_cnt++] = atoi(nr);
                     nr_it = 0;
                     }
                     }
                     else {
                         nr[nr_it++] = s[s_it];
                         }
                         s_it++;
                         }
                         if (nr_it > 0) {
                             nr[nr_it] = '\0';
                             calc.n[nr_cnt] = atoi(nr);
                             }
                             }
                             */

//[LED] [STATE] [MILIS]
void loop() {
    char c;
    if (Serial.available() > 0) {
        c = Serial.read();
        if (idx < (int)sizeof(s) - 1) {
            s[idx++] = c;
        }
        if(c == '\n') {
            process_finished_stream();
        }
    }
    unsigned long nowUs = micros();
    for (int i = 0; i < NUM_LEDS; i++) {
        software_pwm(i, nowUs);
        update_led(i);
    }
}
