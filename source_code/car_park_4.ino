#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Servo.h>
#include "CardChecker.h"

int leftSlot = 4;

int irPin1 = 4;
int irPin2 = 3;
int servoPin = 6;
int ssPin = 9;
int rstPin = 10;

int fullDelay = 2000;
int scanDelay = 400;
int displayDelay = 2000;
int closeDelay = 1000;

// flag for 2 ir sensor, if both are on, then close the gate
int flag1 = 0;
int flag2 = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
CardChecker cardChecker(leftSlot*2, ssPin, rstPin);

void setup() {
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor (0,0);
  lcd.print("  Greenwich VN  ");
  lcd.setCursor (0,1);
  lcd.print("Car Pack Project");
  delay(displayDelay);
  lcd.clear();

  servo.attach(servoPin);
  servo.write(0);

  cardChecker.init();
}

void loop() {
  if (digitalRead(irPin1) == LOW && flag1 == 0) {
    if (leftSlot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        if (cardReadInProcess()) {
          servo.write(90);
          leftSlot--;
        } else {
          flag1 = 0;
        }
      }
    } else {
      lcd.setCursor (0,0);
      lcd.print("    SORRY .-.   ");
      lcd.setCursor (0,1);
      lcd.print("  No slot left  ");
      delay(fullDelay);
      lcd.clear();
    }
    Serial.println("IR1 signal detected.");
  }

  if (digitalRead(irPin2) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      if (cardReadOutProcess()) {
        servo.write(90);
        leftSlot++;
      }
      else {
        flag2 = 0;
      }
    }
    Serial.println("IR2 signal detected.");
  }

  if (flag1 == 1 && flag2 == 1) {
    delay(closeDelay);
    while (digitalRead(irPin1) == LOW || digitalRead(irPin2) == LOW) {
      delay(closeDelay);
    }
    servo.write(0);
    flag1 = 0;
    flag2 = 0;
  }

  lcd.setCursor (0,0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor (0,1);
  lcd.print("Slot Left: ");
  lcd.print(leftSlot);
}

bool cardReadInProcess() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Show your card ");
  lcd.setCursor(0,1);
  String card;
  for (int i = 0; i < 16; i++) {
    lcd.print(".");
    card = cardChecker.readCard();
    if (card != "") {
      cardChecker.addCard(card);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Card accepted ");
      lcd.setCursor(0,1);
      lcd.print("  You can pass  ");
      delay(displayDelay);
      lcd.clear();
      return true;
    }
    delay(scanDelay);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No card detected");
  lcd.setCursor(0,1);
  lcd.print("U shall not pass");
  delay(displayDelay);
  lcd.clear();
  return false;
}

bool cardReadOutProcess() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Show your card ");
  lcd.setCursor(0,1);
  String card;
  for (int i = 0; i < 16; i++) {
    lcd.print(".");
    card = cardChecker.readCard();
    if (card == "") {
      delay(scanDelay);
      continue;
    }
    int cardIndex = cardChecker.checkCard(card);
    if (cardIndex >= 0) {
      cardChecker.removeCard(cardIndex);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Card accepted ");
      lcd.setCursor(0,1);
      lcd.print("  You can pass  ");
      delay(displayDelay);
      lcd.clear();
      return true;
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Wrong card!!  ");
      lcd.setCursor(0,1);
      lcd.print("U shall not pass");
      delay(displayDelay);
      lcd.clear();
      return false;
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No card detected");
  lcd.setCursor(0,1);
  lcd.print("U shall not pass");
  delay(displayDelay);
  lcd.clear();
  return false;
}
