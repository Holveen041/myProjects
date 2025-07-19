#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Adafruit_SSD1306 d(128, 64, &Wire, -1);

int xPos[10];
int yPos[10];
int last=0;

int j=0;
int y=3;

bool isBtn;

int count=0;

int timeResp=0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  for (int i=0;i<10;i++){
    xPos[i]=-1;
    yPos[i]=-1;
  }
}

void delet(int s=0){
  for (int i=s;i<9;i++){
    xPos[i]=xPos[i+1];
    yPos[i]=yPos[i+1];
  }
  xPos[9]=-1;
  yPos[9]=-1;
  last--;
}

void move(){
  for (int i=0;i<10;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      xPos[i]-=2;
      if (xPos[i]<0){
        delet();
        count++;
        i--;
      }
    }
    else break;
  }
  if (j!=0){
    j--;
    y-=2;
  }
  else y+=2;
}

void detectCollision(){
  if (xPos[0]!=-1 and yPos[0]!=-1){
    if ((xPos[0]<8 and (y-3<yPos[0]+40 or y+3>yPos[0]+66)) or (y<3 or y>60)){
      count=0;
      last=0;
      y=3;
      j=0;
      for (int i=0;i<10;i++){
        xPos[i]=-1;
        yPos[i]=-1;
      }
    }
  }
}

void draw(){
  for (int i=0;i<10;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      d.fillRect(xPos[i], yPos[i], 6,40, SSD1306_WHITE);
      d.fillRect(xPos[i], yPos[i]+67,6, 40, SSD1306_WHITE);
    }
    else break;
  }
  d.fillCircle(4, y, 3, 1);
}

void loop() {
  d.clearDisplay();
  
  move();
  draw();
  detectCollision();

  if (digitalRead(6)==0){
    j=5;
    delay(50);
  }

  if (timeResp>1500){
    timeResp=0;
    xPos[last]=120;
    yPos[last]=random(-40,-10);
    last++;
  }

  d.setCursor(100,0);
  d.print("o:");d.print(count);
  d.display();
  timeResp+=70;
  delay(70);
}
