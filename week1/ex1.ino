/*
 *
 R *ealizati un program pentru Arduino care sa primească de la portul serial una din optiunile 1,2 sau 3.

 Dacă opțiunea aleasă este 1 se va afișa pe portul serial cuvântul "pornit" și va aprinde ledul de la pinul 13.
 Dacă opțiunea aleasă este 2 se va afișa pe portul serial cuvântul "oprit" și va stinge ledul de la pinul 13.
 Dacă opțiunea aleasă este 3 se va afișa pe portul serial cuvântul "blink" și va aprinde intermitent ledul de la pinul 13 (asemenea aplicatiei blink).

 Modificați partea de blink pentru a face blink la 10 secunde. De ce nu puteți activa foarte rapid vreuna din celelalte opțiuni dacă stare

 Conditie noua: in timpul in care arduino opereaza cu cele trei stari, putem transmite o operatie dintre doua numere si este printat raspunsul corect

 */

struct Calculator {
  int n[2];
  char op;
};

Calculator calc;
char s[100];
int idx = 0;
int current_num, prev_num;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

inline int is_op(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

inline int is_valid(int n) {
  return n >= 1 && n <= 3;
}

//'NR1 [OP] [NR2]'
void populare_obiect(Calculator& calc) {
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

int compute_res(int n1, int n2, int &res) {
  switch(calc.op) {
    case '+':
      res = n1 + n2;
      break;
    case '-':
      res = n1 - n2;
      break;
    case '*':
      res = n1 * n2;
      break;
    case '/':
      res = n1 / n2;
      break;
    default:
      return -1;
  }
  return 0;
}

void apply_cmd(int cmd) {
  switch (cmd) {
    case 1:
      Serial.println("pornit");
      digitalWrite(13, HIGH);
      break;
    case 2:
      Serial.println("oprit");
      digitalWrite(13, LOW);
      break;
    case 3:
      Serial.println("blink");
      break;
    default:
      break;
  }
}

void process_finished_stream() {
  memset(&calc, 0, sizeof(struct Calculator));
  populare_obiect(calc);
  if(calc.op != 0) {
    int res;
    int code = compute_res(calc.n[0], calc.n[1], res);
    if(!code) {
      char buffer[40];
      sprintf(buffer, "%d %c %d = %d", calc.n[0], calc.op, calc.n[1], res);
      //daca suntem pe 'blink', rez o sa intarzie cu doua secunde, din cauza delay-ului
      Serial.println(buffer);
    }
    else {
      Serial.println(" operatie invalida ");
    }
  }
  else {
    current_num = calc.n[0];
    if(!is_valid(current_num)) {
      current_num = prev_num;
    }
    else {
      prev_num = current_num;
    }
    apply_cmd(current_num);
  }
  memset(s, 0, sizeof(s));
  idx = 0;
}

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
  if(current_num == 3) {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
  }
}
