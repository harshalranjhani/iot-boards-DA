const int trigpin = 7; 
const int echopin = 7;  
const int ledPin = 8;   
const int buzzerPin = 9; 

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sensorValue = 0;
long duration;
int cm = 0;

long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void setup() {
  pinMode(A0, INPUT);   
  pinMode(ledPin, OUTPUT);  
  pinMode(buzzerPin, OUTPUT); 
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(A0);
  
  duration = readUltrasonicDistance(trigpin, echopin);
  cm = 0.01723 * duration;  

  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  Serial.print("Distance in CM: ");
  Serial.println(cm);

  if (sensorValue < 100 && cm < 100) {
    lcd.setCursor(0, 0);
    lcd.print("Dark & Near");
    digitalWrite(ledPin, HIGH);  
    digitalWrite(buzzerPin, HIGH); 
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Light or Far");
    digitalWrite(ledPin, LOW);   
    digitalWrite(buzzerPin, LOW); 
  }
  
  delay(1000);
  lcd.clear();
  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("Distance in CM");
  lcd.setCursor(0, 1);
  lcd.print(cm);
  delay(1000);
  lcd.clear();
  delay(500);
}
