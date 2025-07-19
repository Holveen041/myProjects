#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENC_A_1 2
#define ENC_B_1 4 
#define ENC_A_2 3
#define ENC_B_2 5
#define ENC_TYPE 1

Adafruit_SSD1306 d(128, 64, &Wire, -1);

volatile uint8_t val1 = 11, val2 = 11;
volatile boolean state0_1, lastState_1, turnFlag_1;
volatile boolean state0_2, lastState_2, turnFlag_2;

uint8_t count1 = 0;
uint8_t count2 = 0;

uint8_t xBall = 62;
uint8_t yBall = 29;
float speed = 2;
int angle = 45;
const uint8_t paddleHeight = 10;
const uint8_t ballRadius = 3;

void int0() {
  state0_1 = digitalRead(ENC_A_1);
  if (state0_1 != lastState_1) {
    #if (ENC_TYPE == 1)
      turnFlag_1 = !turnFlag_1;
      if (turnFlag_1)
        val1 += (digitalRead(ENC_B_1) != lastState_1) ? -1 : 1;
    #else
      val1 += (digitalRead(ENC_B_1) != lastState_1) ? -1 : 1;
    #endif
    lastState_1 = state0_1;
  }
  val1 = constrain(val1, 11, 59);
}

void int1() {
  state0_2 = digitalRead(ENC_A_2);
  if (state0_2 != lastState_2) {
    #if (ENC_TYPE == 1)
      turnFlag_2 = !turnFlag_2;
      if (turnFlag_2)
        val2 += (digitalRead(ENC_B_2) != lastState_2) ? -1 : 1;
    #else
      val2 += (digitalRead(ENC_B_2) != lastState_2) ? -1 : 1;
    #endif
    lastState_2 = state0_2;
  }
  val2 = constrain(val2, 11, 59);
}

void setup() {
  Serial.begin(9600);
  pinMode(ENC_A_1, INPUT_PULLUP);
  pinMode(ENC_B_1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A_1), int0, CHANGE);
  pinMode(ENC_A_2, INPUT_PULLUP);
  pinMode(ENC_B_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A_2), int1, CHANGE);
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  randomSeed(analogRead(A0));
}

void resetBall() {
  xBall = 62;
  yBall = 29;
  angle = random(0, 2) ? 45 : 135;
  speed=2;
}

void loop() {
  d.clearDisplay();
  d.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  d.drawLine(64, 0, 64, 10, SSD1306_WHITE);
  d.setCursor(0, 1);
  d.print("P1: ");
  d.print(count1);
  d.setCursor(80, 1);
  d.print("P2: ");
  d.print(count2);

  if (yBall > 58 || yBall < 16) angle = -angle;
  
  if (xBall > 124) {
    if (yBall > val2 && yBall < val2 + paddleHeight) {
      angle = 180 - angle + random(-10, 10);
      speed+=0.15;
    } else {
      count1++;
      resetBall();
    }
  }

  if (xBall < 4) {
    if (yBall > val1 && yBall < val1 + paddleHeight) {
      angle = 180 - angle + random(-10, 10);
      speed+=0.15;
    } else {
      count2++;
      resetBall();
    }
  }

  xBall += cos(angle * PI / 180) * speed;
  yBall += sin(angle * PI / 180) * speed;

  d.drawLine(0, val1, 0, val1 + paddleHeight, SSD1306_WHITE);
  d.drawLine(127, val2, 127, val2 + paddleHeight, SSD1306_WHITE);
  d.fillCircle(xBall, yBall, ballRadius, SSD1306_WHITE);

  d.display();
  delay(50);
}