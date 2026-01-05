#ifndef PRINT_HPP
#define PRINT_HPP

#include <string>
#include <vector>
#include <atomic>
#include "card.hpp"


void waiting(std::atomic<bool> &stop, std::string forwhat = "");

// Prints for Card
int getWidth(Card &c);
void printName(Card &c, bool full = true);
void printStats(Card &c);
void printCard(Card &c);

// Prints for Vectors
void printVec(const std::vector<std::string> &vec);
void printVecCard(std::vector<Card> &vec);

// Separation Prints
void printSeparation();
void printMidSeparation();
void printHugeSeparation();
void printBorderSeparation();

#endif