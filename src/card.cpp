#include "card.hpp"

Card::Card(std::string name, int atk, int def, std::string effect, int level)
  : name_{name}, atk_{atk}, def_{def}, effect_{effect}, level_{level} {};


std::string Card::getName(){
  return name_;
}
int Card::getAtk(){
  return atk_;
}
int Card::getDef(){
  return def_;
}
std::string Card::getEffect(){
  return effect_;
}
int Card::getLevel(){
  return level_;
}