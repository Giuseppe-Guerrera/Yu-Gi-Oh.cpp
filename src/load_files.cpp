#include <iostream>
#include <fstream>
#include <filesystem>

#include "card.hpp"
#include "load_files.hpp"

const char CSV_SEPARATOR = '|';

const std::string DEFAULT_DECKS_FOLDER_PATH = "Decks";
const std::string DEFAULT_DECK_EXTENSION = ".deck";
const std::string DEFAULT_PROFILE_EXTENSION = ".profile";
const std::string DEFAULT_OPPONENTS_EXTENSION = ".opponent";

const std::string CONSTS::DEFAULT_CARDS_PATH = "AllCards";

std::vector<Card> LoadCards(std::map<std::string, Card>& container, std::string path){ //path = CONST::DEFAULT_CARDS_PATH
  std::vector<Card> Deck;
  std::ifstream cardsfile;
  std::string buffer;
  std::vector<std::string> cardComponents(6, " ");
  cardsfile.open(path, std::ios::in);
  if(cardsfile.is_open()){
    while(std::getline(cardsfile, buffer)){
      std::stringstream s(buffer);
      if(buffer[0] == '{'){
        // Class declaration
      }else if(buffer[0] == ' ' || buffer.empty() || buffer[0] == '\0' || buffer[0] == '\r'){
        //  This differs from Windows and Linux
      }else{
        int pos = 0;
        while(std::getline(s, cardComponents[pos++], CSV_SEPARATOR) ){
          // Tokenizing card in its components
        }
        container.emplace(cardComponents[0], Card{cardComponents[0], std::stoi(cardComponents[1]), std::stoi(cardComponents[2]), cardComponents[3], std::stoi(cardComponents[4])} );
        Deck.push_back(Card{cardComponents[0], std::stoi(cardComponents[1]), std::stoi(cardComponents[2]), cardComponents[3], std::stoi(cardComponents[4])} );
        //std::cout << "Added to Deck: " << Deck.back().getName() << ' ' << Deck.back().getAtk() << ' ' << Deck.back().getDef() << ' ' << Deck.back().getEffect() << " LEVEL " << Deck.back().getLevel() << '\n';
      }
    }
  }else{
    std::cerr << "Error when opening the file '" << path << "'\n";
  }
  cardsfile.close();

  return Deck;
}

std::vector<std::string> LoadDeck(std::string path, bool addDefault){  //path = CONST::defaultDeckPath, addDefault = false
  if(addDefault){
    path = DEFAULT_DECKS_FOLDER_PATH + '/' + path;
  }
  path += DEFAULT_DECK_EXTENSION;
  std::ifstream cardsfile;
  std::string buffer;
  std::vector<std::string> sDeck;
  cardsfile.open(path, std::ios::in);
  if(cardsfile.is_open()){
    while(std::getline(cardsfile, buffer)){
      if(buffer.back() == '\r'){  // Files on Windows add '\r' at the end, so we need to remove it on Linux
        buffer.erase(buffer.size()-1);
      }
      if(buffer.back() == ','){
        buffer.erase(buffer.size()-1);
      }
      std::cout << "My Deck: " << buffer << '\n';
      sDeck.emplace_back(buffer);
    }
  }else{
    std::cerr << "Error when opening the file!\n";
  }
  cardsfile.close();
  return sDeck;
}

void LoadMyDeck(const std::map<std::string, Card>& reference,const std::vector<std::string>& inputDeck, std::vector<Card>& v){
  for(std::string s : inputDeck){
    if(reference.contains(s)){
      v.emplace_back(reference.at(s));
    }else{
      std::cerr<< s << " IS NOT PRESENT IN THE \"" << CONSTS::DEFAULT_CARDS_PATH << "\" FILE, THIS ISN'T A RECOGNIZED CARD AND THEREFORE IT CAN'T BE ADDED TO THE DECK\n";
    }
  }
}

void AvaiableDecks(std::vector<std::string>& v){
  for (const auto & entry : std::filesystem::directory_iterator(DEFAULT_DECKS_FOLDER_PATH)){
    if(entry.path().extension() ==  DEFAULT_DECK_EXTENSION){
      v.emplace_back(entry.path().stem().string());
    }
  }
}