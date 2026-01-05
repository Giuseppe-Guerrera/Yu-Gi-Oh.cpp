#ifndef KING_HPP
#define KING_HPP

#include <string>
#include <vector>

#include "card.hpp"
#include "deck.hpp"

struct Kingdom_Struct{
  std::vector<Card> evocation_zone;
  std::vector<Card> artefact_zone;
  std::vector<Card> support_zone;

  void printAll();
  bool isEmpty();
};

class King{
  std::string name_;
  int hp_ {4000};
  Deck deck_;
  std::vector<Card> hand_;
  Kingdom_Struct kingdom_;
  
  public:
    King();
    King(Deck deck);

    //Getters
    std::string getName();
    int getHp();
    Deck& getDeck();
    std::vector<Card>& getHand();
    std::vector<Card>& getEvocationZone();

    int getAtk(int pos);
    int getDef(int pos);
    std::string getLastSummon();

    //Setters
    void setName(std::string s);
    void setDeck(Deck &D);


    void reduceHp(int dmg);
    void moveCardToHand(Card &c);
    void drawCard();
    void summon(int pos);
    void printKingdom();
    void destroyCard(int pos);
    bool isEmpty();
};

#endif