#include <Keypad.h>
#include <Servo.h>

const uint8_t LEDS = 12;
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

// Pin assignments for LED and Buzzer
const int ledPin = 12;    // Pin for LED
const int buzzerPin = 5;  // Pin for Buzzer

void setup() 
{
  // Initialize LED and Buzzer
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);    // Make sure LED is off at the start
  digitalWrite(buzzerPin, LOW); // Make sure Buzzer is off at the start

  lcd.begin(16, 2);
  lcd.print("Enter a Key:-");
  myservo.attach(28);
  
}

void loop()
{
  int pos;
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    Code[keycount] = customKey;
    Serial.print(Code[keycount]);
    lcd.setCursor(5, 1);
    lcd.print(Code[keycount]);
    keycount++;
  }
  
  if (keycount == codeLength - 1)
  {
    Serial.println("");
    if (!strcmp(Code, PassW))  // Correct password
    {
      Serial.println("Correct Password");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("correct Password");

      // Blink the LED when the door opens
      for (pos = 90; pos <= 180; pos++)
      {
        myservo.write(pos);
        digitalWrite(ledPin, HIGH);  // Turn LED on
        delay(100);
        digitalWrite(ledPin, LOW);   // Turn LED off
        delay(100);
      }
      delay(1000);
      
      for (pos = 180; pos >= 90; pos--)
      {
        myservo.write(pos);
        delay(40);
      }

      resetDisplay();  // Reset the LCD display
    }
    else  // Incorrect password
    {
      Serial.println("Incorrect Password");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Incorrect Password");

      // Turn on the buzzer for incorrect password
      digitalWrite(buzzerPin, HIGH);
      delay(1000);  // Buzzer stays on for 1 second
      digitalWrite(buzzerPin, LOW);
      
      delay(1000);  // Wait for 1 second before resetting

      resetDisplay();  // Reset the LCD display to the default message
    }

    deletecount();  // Reset password entry
  }
}

void deletecount()
{
  while (keycount != 0)
  {
    Code[keycount--] = 0;
  }
}

void resetDisplay()
{
  lcd.clear();  // Clear the display
  lcd.setCursor(0, 0);
  lcd.print("Enter a Key:-");  // Set the default message again
}
