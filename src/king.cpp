#include "king.hpp"
#include "print.hpp"

//Kingdom_Struct
  void Kingdom_Struct::printAll(){
    printVecCard(evocation_zone);
  }
  bool Kingdom_Struct::isEmpty(){
    return evocation_zone.empty();
  }

//King Class
  King::King(){}
  King::King(Deck deck){
    deck_ = deck;
  }

  //Getters
  std::string King::getName(){ return name_; }
  int King::getHp(){ return hp_; }
  Deck& King::getDeck(){ return deck_; }
  std::vector<Card>& King::getHand(){ return hand_; }
  std::vector<Card>& King::getEvocationZone(){ return kingdom_.evocation_zone; }

  std::string King::getLastSummon(){
    return kingdom_.evocation_zone.back().getName();
  }
  int King::getAtk(int pos){
    return kingdom_.evocation_zone[pos].getAtk();
  }
  int King::getDef(int pos){
    return kingdom_.evocation_zone[pos].getDef();
  }
  //Setters
  void King::setName(std::string name){
    name_ = name;
  }
  void King::setDeck(Deck &D){
    deck_ = D;
  }
  
  
  void King::reduceHp(int dmg){  
    hp_ = hp_ - dmg;
  }
  
  void King::moveCardToHand(Card &c){ hand_.push_back(std::move(c)); }
  
  void King::drawCard(){
    hand_.push_back(std::move(deck_.getTopCard()) );
    deck_.eraseTopCard();
  }
  
  void King::summon(int pos){
    kingdom_.evocation_zone.push_back(std::move(hand_[pos]));
    hand_.erase(hand_.begin()+pos); 
  }
  
  void King::printKingdom(){
    kingdom_.printAll();
  }
  void King::destroyCard(int pos){
    kingdom_.evocation_zone.erase(kingdom_.evocation_zone.begin()+pos);
  }
  bool King::isEmpty(){
    return kingdom_.isEmpty();
  }