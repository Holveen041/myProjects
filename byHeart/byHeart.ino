#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENC_A 2
#define ENC_B 4 
#define ENC_TYPE 1

volatile int val=1;
volatile boolean state0, lastState, turnFlag;

Adafruit_SSD1306 d(128, 64, &Wire, -1);

int combination[5]={1,2,3,4,5};
int index=0;
int input[5]={0,0,0,0,0};

void int0() {
  state0 = digitalRead(ENC_A);
  if (state0 != lastState) {
    #if (ENC_TYPE == 1)
      turnFlag = !turnFlag;
      if (turnFlag)
        val += (digitalRead(ENC_B) != lastState) ? -1 : 1;
    #else
      val += (digitalRead(ENC_B) != lastState) ? -1 : 1;
    #endif
    lastState = state0;
  }
  val = constrain(val, 1, 5);
}

void shuffle() {
  for (int i = 4; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = combination[i];
    combination[i] = combination[j];
    combination[j] = temp;
  }
}

void change() {
  baze();
  d.setCursor(45, 13);
  d.print("CHOOSE");
  d.fillCircle(20+(val-1)*21, 30, 5,SSD1306_WHITE);
  d.display();
}

void baze(){
  d.clearDisplay();
  for (int i = 0; i <5; i++){
    d.drawLine(20+i*21,35,25+i*21,55,SSD1306_WHITE);
    d.drawLine(20+i*21,35,15+i*21,55,SSD1306_WHITE);
    d.drawCircle(20+i*21, 30, 5,SSD1306_WHITE);
  }
}

void show(){
  for (int i = 0; i <5; i++) {
    digitalWrite(combination[i]+4,1);
    baze();
    d.fillCircle(20+(combination[i]-1)*21, 30, 5,SSD1306_WHITE);
    d.display();
    delay(700);
  }
}

void vict(char text[]){
  d.clearDisplay();
  d.setCursor(15,27);
  d.setTextSize(2);
  d.print(text);
  d.display();
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  attachInterrupt(0, int0, CHANGE);
}

void loop() {
  baze();
  d.setCursor(45,13);
  d.setTextSize(1);
  d.print("START!");
  d.display();
  delay(500);

  shuffle();
  show();

  index=0;
  val=1;
  for (int i = 0; i <5; i++) {
    input[i]=0;
  }

  while (index!=5){
    change();
    if  (digitalRead(6)==0){
      d.fillCircle(20+(val-1)*21, 30, 7,SSD1306_WHITE);
      d.display();
      input[index]=val;
      index+=1;
      delay(200);
    }
    delay(100);
  }

  if (combination[0]==input[0] and combination[1]==input[1] and combination[2]==input[2] and combination[3]==input[3] and combination[4]==input[4]){
    vict("YOU WIN");
    delay(1000);
  }
  else{
    vict("YOU LOSE");
    delay(1000);
  }
}
