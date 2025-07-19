#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 d(128, 64, &Wire, -1);

int r1;
int r2;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
}

void dice(int r, int x,int y){
  d.fillRect(0+x, 10+y, 50, 30, SSD1306_WHITE);
  d.fillRect(10+x, 0+y, 30, 10, SSD1306_WHITE);
  d.fillRect(10+x, 40+y, 30, 10, SSD1306_WHITE);
  d.fillCircle(10+x, 10+y, 10, SSD1306_WHITE);
  d.fillCircle(39+x, 10+y, 10, SSD1306_WHITE);
  d.fillCircle(10+x, 39+y, 10, SSD1306_WHITE);
  d.fillCircle(39+x, 39+y, 10, SSD1306_WHITE);
  if (r==1){
    d.fillCircle(25+x, 25+y, 6, SSD1306_BLACK);
  }
  if (r==2){
    d.fillCircle(17+x, 17+y, 6, SSD1306_BLACK);
    d.fillCircle(34+x, 34+y, 6, SSD1306_BLACK);
  }
  if (r==3){
    d.fillCircle(12+x, 12+y, 6, SSD1306_BLACK);
    d.fillCircle(25+x, 25+y, 6, SSD1306_BLACK);
    d.fillCircle(37+x, 37+y, 6, SSD1306_BLACK);
  }
  if (r==4){
    d.fillCircle(16+x, 16+y, 6, SSD1306_BLACK);
    d.fillCircle(16+x, 34+y, 6, SSD1306_BLACK);
    d.fillCircle(34+x, 16+y, 6, SSD1306_BLACK);
    d.fillCircle(34+x, 34+y, 6, SSD1306_BLACK);
  }
  if (r==5){
    d.fillCircle(14+x, 14+y, 6, SSD1306_BLACK);
    d.fillCircle(14+x, 36+y, 6, SSD1306_BLACK);
    d.fillCircle(36+x, 14+y, 6, SSD1306_BLACK);
    d.fillCircle(36+x, 36+y, 6, SSD1306_BLACK);
    d.fillCircle(25+x, 25+y, 6, SSD1306_BLACK);
  }
  if (r==6){
    d.fillCircle(10+x, 15+y, 6, SSD1306_BLACK);
    d.fillCircle(25+x, 15+y, 6, SSD1306_BLACK);
    d.fillCircle(39+x, 15+y, 6, SSD1306_BLACK);
    d.fillCircle(10+x, 34+y, 6, SSD1306_BLACK);
    d.fillCircle(25+x, 34+y, 6, SSD1306_BLACK);
    d.fillCircle(39+x, 34+y, 6, SSD1306_BLACK);
  }
}

void loop() {
  if (digitalRead(6)==0){
    for (int i=0;i<random(8,20);i++){
      d.clearDisplay();
      r1=random(1,7);
      r2=random(1,7);
      dice(r1,9,12);
      dice(r2,68,12);
      d.display();
      delay(i*50);
    }
    d.setCursor(60,2);
    d.print(r1+r2);
    d.display();
  }
  delay(200);
}
