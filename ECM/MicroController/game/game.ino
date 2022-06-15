#define A 13
#define B 12
#define C 11
#define D 10
#define E 9
#define F 8
#define G 7
#define POINTS_TO_END_THE_GAME 10
// order a b g e d c g f
char order[8] = {A, B, G, E, D, C, G, F};

char pins[7] = {A, B, C, D, E, F, G};
const byte INTERRUPT_PIN = 2;
const byte P1_PIN = 5;
const byte P2_PIN = 6;
size_t totGamePoints = 0;
size_t pointsP1 = 0;
size_t pointsP2 = 0;
volatile bool ended = false;
volatile bool forward = true;
volatile size_t curPos = 0;

void setup() {
  for (size_t i = 0; i <= sizeof(pins); i++) {
    pinMode(pins[i], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), scoring_, RISING);
  Serial.begin(9600);
}

void loop() {
  if (ended) {
    end_();
    return;
  }
  digitalWrite(order[curPos], HIGH);
  delay(500 / (totGamePoints + 1));
  digitalWrite(order[curPos], LOW);
  // select next segment
  curPos = forward ? curPos + 1 : (curPos == 0) ? sizeof(order)-1 : curPos - 1;
  curPos = (curPos) % sizeof(order);
}

void end_() {
  // flash all segments
  for (size_t i = 0; i <= sizeof(pins); i++) {
    digitalWrite(order[i], HIGH);
  }
  delay(1000);
  for (size_t i = 0; i <= sizeof(pins); i++) {
    digitalWrite(order[i], LOW);
  }
  delay(1000);
}

// Prints the current Score of Players
void printScore_() {
  char p1[] = "P1 ";
  char p2[] = "/ P2 ";
  Serial.print(p1);
  Serial.print(pointsP1);
  Serial.print(p2);
  Serial.println(pointsP2);
}

void updateStats_(bool statP1, bool statP2) {
  // check if somebody scored
  if (order[curPos] == A && statP1) {
    totGamePoints = totGamePoints + 1;
    pointsP1 = pointsP1 + 1;
  } else if (order[curPos] == D && statP2) {
    totGamePoints = totGamePoints + 1;
    pointsP2 = pointsP2 + 1;
  } else {
    // penalize player
    if (statP1) {
      pointsP1 = (pointsP1 == 0) ? 0 : pointsP1 - 1;
    }
    if (statP2) {
      pointsP2 = (pointsP2 == 0) ? 0 : pointsP2 - 1;
    }
    // switch direction
    forward = !forward;
  }
}

void scoring_() {
  bool statP1 = digitalRead(P1_PIN);
  bool statP2 = digitalRead(P2_PIN);
  if (ended && statP1 && statP2) {
    // reset the game stats
    totGamePoints = 0;
    pointsP1 = 0;
    pointsP2 = 0;
    ended = false;
    Serial.println("RESET");
    return;
  } else if (ended) {
    // ignore interrupt
    return;
  }
  updateStats_(statP1, statP2);
  // end the game if max points reached
  if (totGamePoints == POINTS_TO_END_THE_GAME) {
    ended = true;
    Serial.println("Final Score:");
  }

  printScore_();
}
