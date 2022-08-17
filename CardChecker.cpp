#include "CardChecker.h"
#include <MFRC522.h>

CardChecker::CardChecker(int cardNum, byte ssPin, byte rstPin) {
  cardList = new String[cardNum];
  this->cardNum = 0;
  MFRC522 mfrc522(ssPin, rstPin);
}

void CardChecker::init() {
  SPI.begin();
  mfrc522.PCD_Init();
}

String CardChecker::readCard() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return "";
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return "";
  }

  // Change UID to String type and return
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  if (content != "") {
     content.substring(1).toUpperCase();
     Serial.println("Card " + content + " recognized.");
  }
  return content;
}

int CardChecker::checkCard(String card) {
  for (int i = 0; i < cardNum; i++) {
    if (card == cardList[i]) {
      Serial.println("Card ID recognized!");
      delay(1000);
      return i;
    }
  }
  Serial.println("Card ID not recognized.");
  delay(1000);
  return -1;
}

void CardChecker::addCard(String card) {
  if (card == "") {
    return;
  }
  cardList[cardNum++] = card;
}

void CardChecker::removeCard(int cardIndex) {
  if (cardIndex < 0 || cardIndex >= cardNum) return;
  cardList[--cardNum] = "";
}
