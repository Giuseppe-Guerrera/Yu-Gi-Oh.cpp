#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include <typeinfo>

#include "print.hpp"
#include "card.hpp"
using std::cout, std::endl;

namespace CONST{
  const int spaceLevel = 3;
}

/**
 * @brief Continuosly prints a text until the value of the atomic changes
 * 
 */
void waiting(std::atomic<bool> &stop, std::string forwhat){
  std::string str = forwhat.length()==0 ? "Waiting" : "Waiting " + forwhat;
  str += "...";
  std::string str_empty(str.length(), ' '); // fill constructor
  while(stop){
    for(int i=0; i<=str.length(); i++){
      if(!stop){
        break;
      }
      cout << "\r" << str.substr(0, i) << str_empty.substr(i,str_empty.length()) <<std::flush;
      std::this_thread::sleep_for(std::chrono::milliseconds(200) );
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200) );
  }
  cout << '\r' << "";
}


int getWidth(Card &c){
  if(c.getName().length()<15){
    return 15;
  }else{
    return c.getName().length();
  }
}
void printName(Card &c, bool full){
  cout << std::setw(getWidth(c)) << c.getName();
  if(full){
    cout << " " << c.getLevel() << "*";
  }
}
void printStats(Card &c){
  int space;
  int width = getWidth(c)+CONST::spaceLevel;
  if(width%2==0){  
    space = 2;
    width = (width/2)-4-1;
  }else{
    space = 1;
    width = (width-1)/2-4;
  }
  cout << "ATK " << std::setw(width) << c.getAtk() << std::setw(space) << ""; //   cout << " ";
  cout << "DEF " << std::setw(width) << c.getDef();//   cout << " ";
}

void printCard(Card &c){
  cout << std::left;
  printName(c);
  cout << endl;
  printStats(c);
  cout << endl << std::resetiosflags(std::ios_base::adjustfield);
}


void printVec(const std::vector<std::string> &vec){
  for(auto c : vec){
    cout << c << " | ";  //cout << std::setw(3) << " | ";
  }
  cout << endl;
}

void printVecCard(std::vector<Card> &vec){
  cout << std::left;
  for(Card c : vec){
    printName(c);
    cout << " | ";  //cout << std::setw(3) << " | ";
  }
  cout << endl;
  for(Card c : vec){
    printStats(c);
    cout << " | ";
  }
  cout << endl;
  cout << std::resetiosflags(std::ios_base::adjustfield);
}


void printSeparation(){
  cout << "--------------------------------------------------------------------------------" << endl;
}
void printMidSeparation(){
  cout << "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" << endl;
}
void printHugeSeparation(){
  cout << "########################################################################################################################" << endl;
}
void printBorderSeparation(){
  cout << "========================================" << endl;
}