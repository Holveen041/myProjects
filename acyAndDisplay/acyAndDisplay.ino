#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 d(128, 64, &Wire, -1);

float angleX = 0, angleY = 0;

int x=62;
int y=30;

void setup() {
  Serial.begin(9600);
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  d.setTextColor(SSD1306_WHITE);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float angleX = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float angleY = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;

  if (angleX>-5 and angleX<5) angleX=0;
  if (angleY>-5 and angleY<5) angleY=0;

  x=constrain(x-(int)angleX/3,5,123);
  y=constrain(y+(int)angleY/3,5,57);

  d.clearDisplay();
  d.fillCircle(x,y,4,SSD1306_WHITE);
  d.display();

  delay(70);
}