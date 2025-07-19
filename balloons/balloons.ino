#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define JOY_X_PIN A2
#define JOY_Y_PIN A3

volatile int val1 = 60;
volatile int val2 = 28;
volatile int joyX, joyY;

Adafruit_SSD1306 d(128, 64, &Wire, -1);

int xPos[20];
int yPos[20];
int last=0;

int count=0;

int timeResp=0;

void readJoystick() {
  joyX = analogRead(JOY_X_PIN);
  joyY = analogRead(JOY_Y_PIN);

  if (joyX<520 and joyX>495) joyX=513;
  if (joyY<520 and joyY>495) joyY=512;

  int deltaX = map(joyX, 0, 1023, -7, 7);
  val1 = constrain(val1 + deltaX, 0, 120);

  int deltaY = map(joyY, 0, 1023, -4, 4);
  val2 = constrain(val2 - deltaY, 0, 56);
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_INVERSE);
  for (int i=0;i<20;i++){
    xPos[i]=-1;
    yPos[i]=-1;
  }
}

void draw(){
  for (int i=0;i<19;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      d.fillCircle(xPos[i]+4, yPos[i]+4, 4, SSD1306_WHITE);
      d.drawLine(xPos[i]+4, yPos[i]+8,xPos[i]+6, yPos[i]+15, SSD1306_WHITE);
      d.drawLine(xPos[i]+6, yPos[i]+15,xPos[i]+2, yPos[i]+20, SSD1306_WHITE);
    }
    else break;
  }
  d.drawCircle(val1+3, val2+3, 3, SSD1306_INVERSE);
}

void delet(int s=0){
  for (int i=s;i<19;i++){
    xPos[i]=xPos[i+1];
    yPos[i]=yPos[i+1];
  }
  xPos[19]=-1;
  yPos[19]=-1;
  last--;
}

void move(){
  int can=0;
  for (int i=0;i<19;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      yPos[i]-=1;
      if (yPos[i]<0){
        can=1;
      }
    }
    else break;
  }
  if (can==1) {
    delet();
    count--;
  }
}

void detectCollision(){
  for (int i=0;i<20;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      if (val1>xPos[i]-3 and val1<xPos[i]+7 and val2>yPos[i]-3 and val2<yPos[i]+7){
        delet(i);
        count++;
        break;
      }
    }
    else break;
  }
}

void loop() {
  d.clearDisplay();

  readJoystick();
  move();
  draw();

  if (timeResp>1000){
    timeResp=0;
    xPos[last]=random(5,120);
    yPos[last]=50;
    last++;
  }
  
  if (digitalRead(6)==0){
    detectCollision();
    delay(100);
  }

  d.setCursor(100,0);
  d.print("o:");d.print(count);

  d.display();
  timeResp+=70;
  delay(70);
}
