void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

}
void loop() {
  if (Serial.available()>0)
  {
    int number = Serial.parseInt();
    switch (number) {
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
        digitalWrite(13, HIGH);
        delay(10000);
        digitalWrite(13, LOW);  
        delay(10000);
        break;
      default:
        Serial.println("illegal");
        break;
    }
    
  }
}
