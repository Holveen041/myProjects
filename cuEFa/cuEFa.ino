#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENC_A 2
#define ENC_B 4 
#define ENC_TYPE 1

volatile int val=1;
volatile boolean state0, lastState, turnFlag;

Adafruit_SSD1306 d(128, 64, &Wire, -1);

int c=0;
int e=0;
int count1=0;
int count2=0;

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
  val = constrain(val, 1, 3);
}

void showScore(){
  d.setCursor(115,15);
  d.print(count1);
  d.setCursor(115,45);
  d.print(count2);
}

void change(){
  d.clearDisplay();
  showScore();
  d.fillCircle(30, 30, 10,SSD1306_WHITE);
  d.fillCircle(30, 35, 10,SSD1306_WHITE);
  d.drawLine(53,20,65,45,SSD1306_WHITE);
  d.drawLine(62,20,50,45,SSD1306_WHITE);
  d.fillCircle(50,45, 2,SSD1306_WHITE);
  d.fillCircle(65,45, 2,SSD1306_WHITE);
  d.fillRect(78,20,15,28,SSD1306_WHITE);
  d.fillCircle(30+(val-1)*27,10,3,SSD1306_WHITE);
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  attachInterrupt(0, int0, CHANGE);
}

void loop() {
  c=0;
  while (true){
    change();
    d.display();
    if (digitalRead(6)==0){
      c=val;
      break;
    }
    delay(100);
  }
  e=random(1, 4);
  for (int i=1;i<8;i++){
    d.clearDisplay();
    showScore();
    d.fillCircle(64, 15, 7+i%2*2,SSD1306_WHITE);
    d.fillCircle(64, 18+i%2, 7+i%2*2,SSD1306_WHITE);
    d.fillCircle(64, 48+i%2, 7+i%2*2,SSD1306_WHITE);
    d.fillCircle(64, 45, 7+i%2*2,SSD1306_WHITE);
    d.display();
    delay(300);
  }
  d.clearDisplay();
  if (e==1){
    d.fillCircle(64, 15, 7,SSD1306_WHITE);
    d.fillCircle(64, 18, 7,SSD1306_WHITE);
  }
  else if (e==2){
    d.drawLine(53,8,65,28,SSD1306_WHITE);
    d.drawLine(62,8,50,28,SSD1306_WHITE);
    d.fillCircle(50,8, 2,SSD1306_WHITE);
    d.fillCircle(65,8, 2,SSD1306_WHITE);
  }
  else{
    d.fillRect(57,7,15,20,SSD1306_WHITE);
  }
  if (c==1){
    d.fillCircle(64, 48, 7,SSD1306_WHITE);
    d.fillCircle(64, 45, 7,SSD1306_WHITE);
  }
  else if (c==2){
    d.drawLine(53,35,65,55,SSD1306_WHITE);
    d.drawLine(62,35,50,55,SSD1306_WHITE);
    d.fillCircle(50,55, 2,SSD1306_WHITE);
    d.fillCircle(65,55, 2,SSD1306_WHITE);
  }
  else{
    d.fillRect(57,40,15,20,SSD1306_WHITE);
  }

  if (c==e){
    count1++;
    count2++;
  }
  else if (e==1 and c==2) count1++;
  else if (e==1 and c==3) count2++;
  else if (e==2 and c==1) count2++;
  else if (e==2 and c==3) count1++;
  else if (e==3 and c==1) count1++;
  else if (e==3 and c==2) count2++;
  showScore();

  d.display();
  delay(1000);
}