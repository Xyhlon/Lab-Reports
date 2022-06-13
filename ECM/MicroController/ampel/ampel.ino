
#define A2_G 10
#define A2_R 9
#define A1_R 13
#define A1_Y 12
#define A1_G 11
#define LIGHT_SENSOR 19



char pins[5] = {A2_G, A2_R, A1_R, A1_Y, A1_G};
const byte interruptPin = 2;
volatile bool pressed = false;
volatile bool night = false;

int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value


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


 // calibrate during the first five seconds
  while (millis() < 5000) {
    sensorValue = analogRead(LIGHT_SENSOR);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
  Serial.begin(9600);
}
void loop() {
    sensorValue = analogRead(LIGHT_SENSOR);
    // in case the sensor value is outside the range seen during calibration
    sensorValue = constrain(sensorValue, sensorMin, sensorMax);

    // apply the calibration to the sensor reading
    sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
    Serial.println(sensorValue);
    if (sensorValue < 100 && !night){
      digitalWrite(A1_G,LOW);
      digitalWrite(A1_R,LOW);
      digitalWrite(A2_G,LOW);
      digitalWrite(A2_R,LOW);
      night = true;
    }
    if (sensorValue > 150)
    {
      digitalWrite(A1_G,HIGH);
      digitalWrite(A1_R,LOW);
      digitalWrite(A2_G,LOW);
      digitalWrite(A2_R,HIGH);
      night = false;
      pressed = false;
    }
    if (night){
        digitalWrite(A1_Y,HIGH);
        delay(1000);
        digitalWrite(A1_Y,LOW);
        delay(1000);
      return;
    }
    
    if (pressed){
    // Green Blinking phase
      for (size_t i = 0; i < 4; i++) {
        digitalWrite(A1_G,LOW);
        delay(1000);
        digitalWrite(A1_G,HIGH);
        delay(1000);
      } 
      digitalWrite(A1_G,LOW);
    // Yellow phase
      digitalWrite(A1_Y,HIGH);
      delay(1000);
    // GO phase
      digitalWrite(A1_Y,LOW);
      digitalWrite(A1_R,HIGH);
      digitalWrite(A2_R,LOW);
      digitalWrite(A2_G,HIGH);
      delay(5000);
    // Green Blinking phase for walkers
      for (size_t i = 0; i < 4; i++) {
        digitalWrite(A2_G,LOW);
        delay(1000);
        digitalWrite(A2_G,HIGH);
        delay(1000);
      } 
      digitalWrite(A2_G,LOW);
    // STOP for walkers and Transition Car
      digitalWrite(A2_R,HIGH);
      digitalWrite(A1_Y,HIGH);
      delay(1000);
    // Car go space
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
