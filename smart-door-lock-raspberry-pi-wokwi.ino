#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define codeLength 5
Servo myservo;
char Code[codeLength];
char PassW[codeLength] = "1234";
byte keycount = 0;

const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

LiquidCrystal lcd(13,11,9,8,7,6);
uint8_t rowPins[ROWS] = { 26, 22, 21, 20 }; 
uint8_t colPins[COLS] = { 19, 18, 17, 16 }; 

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int ledPin = 12;
const int buzzerPin = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  lcd.begin(16, 2);
  lcd.print("Enter a Key:-");
  myservo.attach(28);
}

void loop() {
  int pos;
  char customKey = customKeypad.getKey();
  if (customKey) {
    Code[keycount] = customKey;
    Serial.print(Code[keycount]);
    lcd.setCursor(5, 1);
    lcd.print(Code[keycount]);
    keycount++;
  }
  
  if (keycount == codeLength - 1) {
    Serial.println("");
    if (!strcmp(Code, PassW)) {
      Serial.println("Correct Password");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("correct Password");

      for (pos = 90; pos <= 180; pos++) {
        myservo.write(pos);
        digitalWrite(ledPin, HIGH);
        delay(100);
        digitalWrite(ledPin, LOW);
        delay(100);
      }
      delay(1000);
      
      for (pos = 180; pos >= 90; pos--) {
        myservo.write(pos);
        delay(40);
      }

      resetDisplay();
    } else {
      Serial.println("Incorrect Password");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Incorrect Password");

      digitalWrite(buzzerPin, HIGH);
      delay(1000);
      digitalWrite(buzzerPin, LOW);
      
      delay(1000);

      resetDisplay();
    }

    deletecount();
  }
}

void deletecount() {
  while (keycount != 0) {
    Code[keycount--] = 0;
  }
}

void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter a Key:-");
}
