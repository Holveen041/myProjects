#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 d(128, 64, &Wire, -1);

float angleY = 0;

int combination[4]={0,1,2,3};
int xPos[10];
int yPos[10];
int last=0;

int y;

int count=0;

int timeResp=0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  for (int i=0;i<10;i++){
    xPos[i]=-1;
    yPos[i]=-1;
  }
}

void shuffle() {
  for (int i = 3; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = combination[i];
    combination[i] = combination[j];
    combination[j] = temp;
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
  int can=0;
  for (int i=0;i<10;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      xPos[i]-=2;
      if (xPos[i]<-5){
        can=1;
        delet(i);
        i--;
      }
    }
    else break;
  }
  if (can==1) {
    count++;
  }
}

void draw(){
  for (int i=0;i<10;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      d.fillRect(xPos[i], yPos[i], 20,12, SSD1306_WHITE);
    }
    else break;
  }
  d.fillRect(4, y, 20, 12,1);
}

void detectCollision(){
  for (int i=0;i<3;i++){
    if (xPos[i]!=-1 and yPos[i]!=-1){
      if (xPos[i]<26 and yPos[i]>y-12 and yPos[i]<y+12){
        count=0;
        last=0;
        for (int i=0;i<10;i++){
          xPos[i]=-1;
          yPos[i]=-1;
        }
        break;
      }
    }
    else break;
  }
}

void loop() {
  d.clearDisplay();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float angleY = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
  if (angleY>-5 and angleY<5) angleY=0;
  y=constrain(y+(int)angleY/3,0,50);

  move();
  draw();
  detectCollision();

  if (timeResp>2400){
    timeResp=0;
    shuffle();
    for (int i=0;i<random(1,4);i++){
      xPos[last]=108;
      yPos[last]=combination[i]*16;
      last++;
    }
  }

  d.setCursor(100,0);
  d.print("o:");d.print(count);
  d.display();
  timeResp+=70;
  delay(70);
}
