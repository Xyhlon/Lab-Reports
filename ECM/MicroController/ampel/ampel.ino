
#define A2_G 10
#define A2_R 9
#define A1_R 13
#define A1_Y 12
#define A1_G 11



char pins[5] = {A2_G,A2_R, A1_R, A1_Y, A1_G};
const byte interruptPin = 2;
volatile bool pressed = false;


void setup() {
  for (size_t i = 0; i <= sizeof(pins); i++) {
    pinMode(pins[i], OUTPUT);
  } 
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleRequest, RISING);
  digitalWrite(A1_G,HIGH);
  digitalWrite(A1_Y,LOW);
  digitalWrite(A1_R,LOW);
  digitalWrite(A2_G,LOW);
  digitalWrite(A2_R,HIGH);

  Serial.begin(9600);
}
void loop() {
    if (pressed){
      for (size_t i = 0; i < 4; i++) {
        digitalWrite(A1_G,LOW);
        delay(1000);
        digitalWrite(A1_G,HIGH);
        delay(1000);
      } 
      digitalWrite(A1_G,LOW);
      digitalWrite(A1_Y,HIGH);
      delay(1000);
      digitalWrite(A1_Y,LOW);
      digitalWrite(A1_R,HIGH);
      digitalWrite(A2_R,LOW);
      digitalWrite(A2_G,HIGH);
      delay(5000);
      for (size_t i = 0; i < 4; i++) {
        digitalWrite(A2_G,LOW);
        delay(1000);
        digitalWrite(A2_G,HIGH);
        delay(1000);
      } 
      digitalWrite(A2_G,LOW);
      digitalWrite(A2_R,HIGH);
      digitalWrite(A1_Y,HIGH);
      delay(1000);
      digitalWrite(A1_Y,LOW);
      digitalWrite(A1_R,LOW);
      digitalWrite(A1_G,HIGH);
      pressed = false;
    }
}
void handleRequest(){
  if (!pressed){
    pressed = true;
  }
}
