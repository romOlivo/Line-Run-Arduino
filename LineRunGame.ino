// include the library code:
#include <LiquidCrystal.h>
#include "pitches.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int button = 6;
int buzzer = 5;

int nCol = 16;
int nRow = 2;
int i, j, playerPosition, score, timePassed;
int obstacles[10][2] = {
  { 16, rand() % 2 }, 
  { 23, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }, 
  { rand() % 7 + 15, rand() % 2 }
};
int nObstacles;
bool gameEnd;
int nota;

char * strBlank = "";
char text[16];

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(nCol, nRow);
  // Print a message to the LCD.
  lcd.print("Line Run");
  lcd.setCursor(0, 1);
  lcd.print("Start!");
  playerPosition = 1;
  nObstacles=0;
  new_obstacle();
  score = 0;
  timePassed = 0;
  pinMode(button, INPUT_PULLUP);
  initial_music();
  delay(1100);
  gameEnd = false;
}

void loop() {
  if (!gameEnd){
    nota = NOTE_F5;
    timePassed++;
    clean_screen();
    print_player();
    update_obstacles();
    print_obstacles();
    update_number_of_obstacles();
    tone(buzzer, nota, 40);
    delay(400);
  }
}

char * string_repeat( int n, const char * s ) {
  size_t slen = strlen(s);
  char * dest = malloc(n*slen+1);
 
  int i; char * p;
  for ( i=0, p = dest; i < n; ++i, p += slen ) {
    memcpy(p, s, slen);
  }
  *p = '\0';
  return dest;
}

void clean_screen() {
  if (strBlank == ""){
    strBlank = string_repeat(nCol, " ");
  }
  for(i=0;i<nRow;i++) {
    lcd.setCursor(0, i);
    lcd.print(strBlank);
  }
}

void print_player() {
  if(gameEnd){
    return;
  }
  if (digitalRead(button) == LOW) {
    if(playerPosition == 1){
      nota = NOTE_A5;
    }
    playerPosition = 0;
  } else {
    if(playerPosition == 0){
      nota = NOTE_E5;
    }
    playerPosition = 1;
  }
  lcd.setCursor(0, playerPosition);
  lcd.print("X");
}

void print_obstacles() {
  if(gameEnd){
    return;
  }
  for(i=0;i<nObstacles;i++){
    lcd.setCursor(obstacles[i][0], obstacles[i][1]);
    lcd.print("*");
  }
}

void update_obstacles() {
  for (i=0;i<nObstacles;i++){
    obstacles[i][0]--;
    if (obstacles[i][0] < 0) {
      while (new_obstacle_position()){
        continue;
      }
    } else if (obstacles[i][0] == 0) {
      if (obstacles[i][1] == playerPosition) {
        end_game();
        break;
      } else {
        obstacle_avoided();
      }
    }
  }
}

void update_number_of_obstacles(){
  if (nObstacles == 1){
    new_obstacle();
  } else if (nObstacles == 10) {
    return;
  }
  switch(timePassed) {
    case 12:
    case 50:
    case 120:
    case 160:
    case 210:
    case 280:
    case 360:
    case 480:
      new_obstacle();
      break;
  }
}

void new_obstacle() {
  Serial.println("New obstacle created!");
  i = nObstacles;
  nObstacles++;
  while(new_obstacle_position())
    continue;
}

void end_game() {
  gameEnd = true;
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  sprintf(text, "Score: %i", score);
  lcd.print(text);
  tone(buzzer, NOTE_D5, 500);
  delay(400);
  tone(buzzer, NOTE_C5, 500);
  delay(400);
  tone(buzzer, NOTE_D5, 500);
  delay(600);
  tone(buzzer, NOTE_C5, 500);
  delay(600);
  nota = NOTE_C5;
}

void obstacle_avoided() {
  score++;
}

bool new_obstacle_position() {
  obstacles[i][0] = rand() % 7 + 15;
  for (j=0;j<nObstacles;j++){
    if (i != j && (obstacles[i][0] == obstacles[j][0] || obstacles[i][0] + 1 == obstacles[j][0]
    || obstacles[i][0] - 1 == obstacles[j][0])){
      return true;
    }
  }
  return false;
}

void initial_music(){
  tone(buzzer, NOTE_E5, 500);
  delay(500);
  tone(buzzer, NOTE_F5, 500);
  delay(500);
  tone(buzzer, NOTE_G5, 500);
  delay(500);
}
