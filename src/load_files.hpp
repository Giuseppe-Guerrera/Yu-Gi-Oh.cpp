#ifndef LOAD_FILES_HPP
#define LOAD_FILES_HPP

#include <string>
#include <vector>
#include <map>
#include "card.hpp"

namespace CONSTS{
  extern const std::string DEFAULT_CARDS_PATH;
}

std::vector<Card> LoadCards(std::map<std::string, Card>& container, std::string path = CONSTS::DEFAULT_CARDS_PATH);

std::vector<std::string> LoadDeck(std::string path = CONSTS::DEFAULT_CARDS_PATH, bool addDefault = false);

void LoadMyDeck(const std::map<std::string, Card>& reference,const std::vector<std::string>& inputDeck, std::vector<Card>& v);

void AvaiableDecks(std::vector<std::string>& v);

#endif