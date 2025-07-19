#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENC_A 2
#define ENC_B 4 
#define ENC_TYPE 1

volatile int val=2;
volatile boolean state0, lastState, turnFlag;

Adafruit_SSD1306 d(128, 64, &Wire, -1);

int xPos[40];
int yPos[40];
int last=0;

int x=0;
int y=0;

int size=4;
int len=1;

int axPos;
int ayPos;

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
  if (val>3){
    val=0;
  }
  if (val<0){
    val=3;
  } 
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  attachInterrupt(0, int0, CHANGE);
  for (int i=0;i<40;i++){
    xPos[i]=-1;
    yPos[i]=-1;
  }
  xPos[last]=x;
  yPos[last]=y;
  createApple();
}

void draw(){
  d.fillRect(axPos+1,ayPos+1,2,2, SSD1306_WHITE);
  for (int i=0;i<40;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      d.fillRect(xPos[i], yPos[i], size, size, SSD1306_WHITE);
    }
    else break;
  }
}

void delet(){
  for (int i=0;i<39;i++){
    xPos[i]=xPos[i+1];
    yPos[i]=yPos[i+1];
  }
  last--;
}

void createApple(){
  while (true){
    axPos=size*random(0,127/size+1);
    ayPos=size*random(0,63/size+1);
    int can=0;
    for (int i=0;i<40;i++){
      if (xPos[i]==axPos and yPos[i]==ayPos){
        can=1;
        break;
      }
    }
    if (can==0) break;
  }
}

void restart(char text[]){
  d.clearDisplay();
    d.setCursor(20,10);
    d.setTextSize(2);
    d.print(text);
    d.display();
    delay(1000);
    for (int i=0;i<40;i++){
      xPos[i]=-1;
      yPos[i]=-1;
    }
    last=0;
    x=0;
    y=0;
    xPos[last]=x;
    yPos[last]=y;
    createApple();
}

void detectApple(){
  if (axPos==xPos[last] and ayPos==yPos[last]){
    if (len<36){
      len++;
      createApple();
    }
  }
  else{
    delet();
  }
  if (len==35){
    restart("YOU WIN!");
  }
}

void detectCollision(){
  int can=0;
  for (int i=0;i<40;i++){
    if (xPos[i]==xPos[last] and yPos[i]==yPos[last] and i!=last){
      can=1;
      break;
    }
  }
  if (can==1){
    restart("YOU LOSE!");
  }
}

void loop() {
  detectCollision();

  d.clearDisplay();
  if (val == 0) y -= size;
  else if (val == 1) x += size;
  else if (val == 2) y += size;
  else if (val == 3) x -= size;
  if (x>128-size) x=0;
  else if (x<0) x=128-size;
  if (y>64-size) y=0;
  else if (y<0) y=64-size;
  yPos[last+1]=y;
  xPos[last+1]=x;
  last++;

  detectApple();

  draw();
  d.display();
  delay(200);
}
