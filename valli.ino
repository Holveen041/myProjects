#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
LiquidCrystal_I2C lcd(0x27,16,2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int index=0;
int pass[10]={0,0,0,0,0,0,0,0,0,0};
int ps[10][10]={{5,4,8,2,4,1,1,0,0,0},{3,9,4,3,8,4,0,0,0,0},{2,0,0,1,2,0,1,2,0,0},{1,1,1,1,1,1,0,0,0,0},{0,6,3,4,0,0,0,0,0,0},{0,6,4,2,0,0,0,0,0,0},{2,0,2,5,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};
int actions[10]={0,0,0,0,1,0,0,0,0,0};
char places[10][32]={"compost pit","electrical panel","Small column","Mirror","","Garden bed in a greenhouse","Stepladder","","",""};

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  myservo.attach(11);
  myservo.write(180);
  lcd.setCursor(0, 0);
  lcd.print("Hi!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void pr(char text[]){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CORECT");
  delay(500);
  lcd.clear();
  int num=0;
  if (strlen(text)<17){
    lcd.setCursor(0,0);
    lcd.print(text);
    delay(50);
  }
  else{
    for (int p=15;p>=0;p--){
      if (text[p]==' '){
        num=p+1;
        break;
      }
    }
    for (int p=0;p<num;p++){
      lcd.setCursor(p,0);
      lcd.print(text[p]);
      delay(50);
    }
    for (int p=num;p<strlen(text);p++){
      lcd.setCursor(p-num,1);
      lcd.print(text[p]);
      delay(25);
    }
  }
  delay(10000);
  lcd.clear();
}

void ch(int text[]){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CORECT");
  delay(500);
  lcd.clear();
  for (int c=0;c<5;c++){
    lcd.setCursor(c,0);
    lcd.write(text[c]);
  }
  delay(10000);
  lcd.clear();
}

void del(){
  if (index!=0){
    index-=1;
    pass[index]=0;
    lcd.clear();
    for (int i=0;i<index;i++){
      lcd.setCursor(i,0);
      lcd.print(pass[i]);
    }
  }
}

void check(){
  int stateCorect=0;

  for (int i=0;i<10;i++){
    if (pass[0]==ps[i][0] and pass[1]==ps[i][1] and pass[2]==ps[i][2] and pass[3]==ps[i][3] and pass[4]==ps[i][4] and pass[5]==ps[i][5] and pass[6]==ps[i][6] and pass[7]==ps[i][7] and pass[8]==ps[i][8] and pass[9]==ps[i][9]){
      if (actions[i]==0){
        pr(places[i]);
        Serial.println(1);
      }
      else if (actions[i]==1){
        myservo.write(0);
        delay(5000);
        myservo.write(180);
        lcd.clear();
      }
      stateCorect=1;
      break;
    }
  }

  if (stateCorect==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("UNCORECT");
    delay(500);
    lcd.clear();
  }

  index=0;
  for (int i=0;i<10;i++){
      pass[i]=0;
  }
}

  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    if (key=='#'){
      del();
    }
    else if (key=='*'){
      check();
    }
    else{
      if (index!=10){
        lcd.setCursor(index,0);
        lcd.print(key);
        pass[index]=key-'0';
        index+=1;
      }
    }
  }
}
