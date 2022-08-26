#ifndef CARD_CHECKER
#define CARD_CHECKER

#include <Arduino.h>
#include <MFRC522.h>

class CardChecker {
  private:
    String *cardList;
    // hold an array of card uid string
    
    int cardNum;
    // hold the number of card added to the card array
    
    MFRC522 mfrc522;
    // the object for controlling the rfid module
    
  public:
    CardChecker(int cardNum, byte ssPin, byte rstPin);
    void init();
    String readCard();
    // use rfid module to read and return uid string
    
    int checkCard(String card);
    // return -1 if not recognized, otherwise return card index
    
    void addCard(String card);
    void removeCard(int cardIndex);
};

#endif
