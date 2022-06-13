
#define A 13
#define B 12
#define C 11
#define D 10
#define E 9
#define F 8
#define G 7
// order a b g e d c g f 
char order[8] = {A,B,G,E,D,C,G,F};

char pins[7] = {A,B,C,D,E,F,G};
const byte interruptPin = 2;
const byte player1pin = 5;
const byte player2pin = 6;
size_t tot_game_points = 0;
size_t points_ply_1 = 0;
size_t points_ply_2 = 0;
volatile bool ended = false;
volatile bool forward = true;



void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  for (size_t i = 0; i <= sizeof(pins); i++) {
    pinMode(pins[i], OUTPUT);
  } 
  attachInterrupt(digitalPinToInterrupt(interruptPin), scoring, RISING);
  Serial.begin(9600);
}

size_t j = 0;

void loop() {
    if (ended){
      end();
      return;
    }
    digitalWrite(order[j],HIGH);
    delay(500/(tot_game_points+1));
    digitalWrite(order[j],LOW);
    /* Serial.println(j); */
    /* Serial.println(sizeof(order)); */
    j = forward ? j+1 : (j==0)? 7:j-1;
    j =(j)%sizeof(order);
}

void end() {
    for (size_t i = 0; i <= sizeof(pins); i++) {
      digitalWrite(order[i],HIGH);
    } 
    delay(1000);
    for (size_t i = 0; i <= sizeof(pins); i++) {
      digitalWrite(order[i],LOW);
    } 
    delay(1000);
}

void scoring() {
  bool stat_p1 =digitalRead(player1pin);
  bool stat_p2 =digitalRead(player2pin);
  if (ended && stat_p1 && stat_p2){
    tot_game_points = 0;
    points_ply_1 =0;
    points_ply_2 =0;
    ended = false;
    Serial.println("RESET");
    return;
  }else if (ended){
    /* end(); */
    return;
  }
  if (order[j]==A && stat_p1){
    tot_game_points = tot_game_points +1;
    points_ply_1 = points_ply_1 +1;
  }else if(order[j]==D && stat_p2){
    tot_game_points = tot_game_points +1;
    points_ply_2 = points_ply_2 +1;
  }
  else{
    if (stat_p1){
      points_ply_1 = (points_ply_1==0) ? 0: points_ply_1 -1;
    }
    if (stat_p2){
      points_ply_2 = (points_ply_2==0) ? 0: points_ply_2 -1;
    }
    forward = !forward;
  }
  if (tot_game_points == 10){
    ended = true;
    Serial.println("Final Score:");
    /* end(); */
  }

  char p1[] = "P1 ";
  char p2[] = "/ P2 ";
  Serial.print(p1);
  Serial.print(points_ply_1);
  Serial.print(p2);
  Serial.println(points_ply_2);
}
