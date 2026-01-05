#include <random>
#include <algorithm>

#include "deck.hpp"

//Deck Class
  //Constructors
  Deck::Deck(){}
  Deck::Deck(std::vector<Card>& vec_param) : vecCard_{vec_param} {}
  Deck::Deck(std::vector<Card>&& vec_param) : vecCard_{vec_param} {}  //Initialization with move semantics

  //Functions
  void Deck::shuffle(){
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::shuffle(vecCard_.begin(), vecCard_.end(), generator);
  }
  int Deck::getSize(){
    return vecCard_.size();
  }
  Card& Deck::getTopCard(){
    return vecCard_.front();
  }
  void Deck::eraseTopCard(){
    vecCard_.erase(vecCard_.begin());
  }
  Card& Deck::draw(){ // TO DO
    // Incomplete do not use
    return vecCard_.front();
  }