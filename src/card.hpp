#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card{
  private:
    std::string name_ {"Default Name"};
    int atk_ {0};
    int def_ {-1};
    std::string effect_ {"Default Effect"};
    int level_ {0};

  public:
    Card() = default;
    Card(std::string name, int atk, int def, std::string effect, int level = 1);
    
    //Getters
    std::string getName();
    int getAtk();
    int getDef();
    std::string getEffect();
    int getLevel();
};

#endif