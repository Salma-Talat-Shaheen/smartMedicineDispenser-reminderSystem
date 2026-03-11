/* * Project Name: Smart Medicine Dispenser & Reminder
 * Developer: Salma T. S. Shaheen | 220210654
 * Version: 23.0 (Fixed LCD Overlap in Mode 4)
 */

#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>

// LCD Connections
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo pillServo; 

// Pin Definitions
const int colorA = 13;   
const int colorB = 8;    
const int actionPin = A0; 
const int modePin = A1;   
const int UP = A2;     
const int DOWN = A3;   
const int servoPin = 9;  

// Time Variables
int h = 8, m = 1, s = 0; 
int alertHH = 8, alertMM = 2; 

// System States
int systemMode = 0; 
bool isAlerting = false;
bool alertDone = false; 
unsigned long lastTick = 0;
int lastDisplayState = -1; // -1: None, 0: Alarm Info, 1: Message

// Sub-mode Logic
bool isTypeSelected = false; 
int selectedType = 0; 

void setup() {
  lcd.begin(16, 2);
  pillServo.attach(servoPin);
  pillServo.write(0); 
  
  pinMode(colorA, OUTPUT);  
  pinMode(colorB, OUTPUT);
  pinMode(actionPin, INPUT);
  pinMode(modePin, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);

  // Identity Screens
  lcd.setCursor(0, 0); lcd.print("SALMA SHAHEEN"); 
  lcd.setCursor(0, 1); lcd.print("220210654");      
  delay(600); lcd.clear();
}

void loop() {
  if (digitalRead(modePin) == HIGH && systemMode != 4) {
    systemMode++;
    if (systemMode > 3) systemMode = 1; 
    isTypeSelected = false; 
    selectedType = 0;
    lcd.clear();
    delay(250); 
  }

  if (digitalRead(actionPin) == HIGH && systemMode == 3) {
    systemMode = 4;
    lcd.clear();
    lcd.print("M4: STARTING...");
    delay(600);
    lcd.clear();
    lastDisplayState = -1; // Reset state for Mode 4
  }

  switch (systemMode) {
    case 0: displayStandby(); break;
    case 1: handleSetting(alertHH, alertMM, "M1: SET ALARM"); break; 
    case 2: handleSetting(h, m, "M2: SET TIME"); break;      
    case 3: displayReady(); break;
    case 4: runClockMode4(); break;
  }
}

void displayStandby() {
  lcd.setCursor(0, 0); lcd.print("MODE 0: STANDBY");
  lcd.setCursor(0, 1); lcd.print("Press MODE Key ");
}

void displayReady() {
  lcd.setCursor(0, 0); lcd.print("MODE 3: READY? ");
  lcd.setCursor(0, 1); lcd.print("PRESS ACTION...");
}

void handleSetting(int &hourVar, int &minVar, String title) {
  if (!isTypeSelected) {
    lcd.setCursor(0, 0); lcd.print(title);
    lcd.setCursor(0, 1); lcd.print("UP:HRS | DN:MIN"); 
    if (digitalRead(UP) == HIGH) { selectedType = 1; isTypeSelected = true; lcd.clear(); delay(300); }
    if (digitalRead(DOWN) == HIGH) { selectedType = 2; isTypeSelected = true; lcd.clear(); delay(300); }
  } 
  else {
    lcd.setCursor(0, 0); lcd.print(title);
    if (digitalRead(UP) == HIGH) {
      if (selectedType == 1) hourVar = (hourVar + 1) % 24;
      else minVar = (minVar + 1) % 60;
      delay(150); 
    }
    if (digitalRead(DOWN) == HIGH) {
      if (selectedType == 1) hourVar = (hourVar + 23) % 24;
      else minVar = (minVar + 59) % 60;
      delay(150); 
    }
    lcd.setCursor(0, 1);
    if (selectedType == 1) lcd.print(">H:"); else lcd.print(" H:");
    printDigits(hourVar);
    lcd.print("   ");
    lcd.setCursor(9, 1);
    if (selectedType == 2) lcd.print(">M:"); else lcd.print(" M:");
    printDigits(minVar);
  }
}

void runClockMode4() {
  // 1. Update Internal Clock Logic
  if (millis() - lastTick > 200) { 
    lastTick = millis();
    s++;
    if (s >= 60) { s = 0; m++; }
    if (m >= 60) { m = 0; h++; }
    if (h >= 24) h = 0;

    // 2. Display Time (Row 0) - Fixed Position
    lcd.setCursor(0, 0);
    lcd.print("TIME: ");
    printDigits(h); lcd.print(":"); printDigits(m); lcd.print(":"); printDigits(s);
    lcd.print("   "); // Clear trailing characters

    // 3. Handle Row 2 Alternating Display
    if (!isAlerting) {
      digitalWrite(colorA, LOW); digitalWrite(colorB, LOW);
      
      int currentState = (s % 10 < 5) ? 0 : 1; // 0 for Alarm info, 1 for Message

      if (currentState != lastDisplayState) {
          lcd.setCursor(0, 1);
          lcd.print("                "); // Clear the whole line once
          lastDisplayState = currentState;
      }

      lcd.setCursor(0, 1);
      if (currentState == 0) {
        lcd.print("ALR AT: "); printDigits(alertHH); lcd.print(":"); printDigits(alertMM);
      } else {
        lcd.print("Get Well, Salma!");
      }
    }

    // 4. Trigger Alarm Logic
    if (h == alertHH && m == alertMM && !alertDone) isAlerting = true;
    else if (m != alertMM) alertDone = false; 
  }

  // 5. Handling Alert State
  if (isAlerting) {
    pillServo.write(90);
    lcd.setCursor(0, 1);
    lcd.print(">> TAKE PILL! <<");
    if ((millis() / 200) % 2 == 0) {
      digitalWrite(colorA, HIGH); digitalWrite(colorB, LOW);
    } else {
      digitalWrite(colorA, LOW); digitalWrite(colorB, HIGH);
    }
  }

  // 6. Stop/Confirm Action
  if (digitalRead(actionPin) == HIGH && isAlerting) {
    isAlerting = false;
    alertDone = true; 
    digitalWrite(colorA, LOW); digitalWrite(colorB, LOW);
    pillServo.write(0);
    lcd.setCursor(0, 1);
    lcd.print("  WELL DONE!    ");
    delay(1500);
    lcd.clear();
    lastDisplayState = -1; // Reset to force refresh
  }
}

void printDigits(int digits) {
  if (digits < 10) lcd.print('0');
  lcd.print(digits);
}