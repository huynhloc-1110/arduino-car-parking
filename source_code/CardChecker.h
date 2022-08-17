#ifndef CARD_CHECKER
#define CARD_CHECKER

#include <Arduino.h>
#include <MFRC522.h>

class CardChecker {
  private:
    String *cardList;
    int cardNum;
    MFRC522 mfrc522;
    
  public:
    CardChecker(int cardNum, byte ssPin, byte rstPin);
    void init();
    String readCard();
    int checkCard(String card); // return -1 if not recognized, otherwise return card index
    void addCard(String card);
    void removeCard(int cardIndex);
};

#endif
