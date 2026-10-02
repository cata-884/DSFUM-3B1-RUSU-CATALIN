void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

enum STATE_CIRCUIT{
  PORNIT,
  OPRIT,
  BLINK,
  INVALID
};

enum OPERATIE{
  CITIRE_NUMAR,
  CITIRE_CARACTERE,
  ALTE_OPERATII
}

struct Calculator{
  int n[2];
  char op;
};

void populare_buffer(){
  c = Serial.read();
  s[idx++] = c;
}

void populare_obiect(Calculator& calc){
  int i = 0, j = 0, k=0;
  char nr[100];
  while(s[i] != '\n'){
    if(s[i] == '+' || s[i] == '-'
    || s[i] == '*' || s[i] == '/'){
      calc.op = s[i++];
    }
    else if(s[i] != ' '){
      nr[j++] = s[i++];
    }
    else{
      calc.n[k++] = atoi(nr);
      memset(nr, 0, 100);
      j=0;
    }
    i++;
  }
}

STATE stare_curenta, stare_anterioara;
OPERATIE operatie_curenta;

char s[100];
int idx = 0;
char c;

void loop() {
  if (Serial.available()>0)
  {

    populare_buffer();

    if(c == '\n')

    if(operatie_curenta == CITIRE_NUMAR) int number = atoi(s);

    switch(number){
      case 1:
        Serial.println("pornit");
        state = PORNIT;
        break;
      case 2:
        Serial.println("oprit");
        state = OPRIT;
        break;
      case 3:
        Serial.println("blink");
        state = BLINK;
        break;
      default:
        break;
    }
    
  }

    switch(state){
      case PORNIT:
        digitalWrite(13, HIGH); 
        break;
      case OPRIT:
        digitalWrite(13, LOW); 
        break;  
      case BLINK:
        digitalWrite(13, HIGH); 
        delay(1000);
        digitalWrite(13, LOW);
        delay(1000); 
        break;   
      case INVALID:
        break; 
    }
}
