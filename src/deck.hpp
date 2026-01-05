#ifndef DECK_HPP
#define DECK_HPP

#include <vector>

#include "card.hpp"

class Deck{
  std::vector<Card> vecCard_;
  public:
  Deck();
  Deck(std::vector<Card>& vec_param);
  Deck(std::vector<Card>&& vec_param);  //Initialization with move semantics

  void shuffle();
  int getSize();
  Card& getTopCard();
  void eraseTopCard();
  Card& draw();
};

#endif