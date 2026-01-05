#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <atomic>
#include <type_traits>

#include "card.hpp"
#include "deck.hpp"
#include "king.hpp"
#include "print.hpp"
#include "load_files.hpp"
#include "async_queue.hpp"
#include "peer.hpp"


const int WAIT_TIME {680};
const int STARTING_CARDS {3};
const Card DEFAULT_CARD{"Unnamed", 0, 0, "Effectless", 0};  //Used for multiplayer


inline void sleepForMs(int x);

char getAnswer();
int pickBase(int size);
template<typename T>
int pick(std::vector<T> &vec, std::string action);

void playRound(King &k1, King &k2, int round, Peer &p);
void playEnemyRound(King &k1, King &k2, AsyncQueue& async_que, std::map<std::string, Card> cards_database);
void printAllKingdoms(King &k1, King &k2);

int main(int argc, char *argv[]){
  std::ios::sync_with_stdio(false);     //Not necessary, used to achieve better performance

  // Choose your name
  King king1;
  King king2;
  std::string tmp;
  std::cout << "Choose a name for your King: ";
  std::getline(std::cin, tmp);
  king1.setName(tmp);
  
  // Connecting to the other player
  std::thread thread_io;
  asio::io_context io_context;
  auto work_guard = asio::make_work_guard(io_context);
  AsyncQueue async_que;
  std::optional<Peer> peer;
  try {
    std::string peer_ip;
    std::cout << "Enter the peer's IP address (leave blank to wait for connection): ";
    std::getline(std::cin, peer_ip);
    peer.emplace(io_context, peer_ip, async_que);
    thread_io = std::thread( [&io_context]() { 
      io_context.run();
      std::cerr << "[WARNING] THE THREAD RUNNING THE CONNECTION STOPPED!!!" << std::endl;
    });
    // Display animation while we wait for the connection
    std::atomic<bool> waiting_running(true);
    std::thread animation_thread(waiting, std::ref(waiting_running), "for connection");
    if(async_que.getValue() == "Connection successful"){
      waiting_running = false;
      animation_thread.join();
    }else{
      std::cerr << "ERROR DURING CONNECTION" << std::endl;
    }
  }catch (std::exception& e) {
    std::cerr << "[ERROR] " << e.what() << std::endl;
    std::cerr << "A critical error occurred while trying to establish a connection"<< std::endl;
    std::cerr << "The program will now exit" << std::endl;
    sleepForMs(1500);
    return 1;
  }

  // Exchanging our names
  peer.value().sendData(king1.getName());
  king2.setName(async_que.getValue());
  std::cout << "\nSuccessfully connected to " << king2.getName() << '\n';

  // Load all the possible cards that you or your opponent can use
  std::map<std::string, Card> cards_database;
  LoadCards(cards_database);
  std::cout << "Successfully loaded " << cards_database.size() << " cards.\n\n";

  // Choose your deck
  std::vector<std::string> decks;
  AvaiableDecks(decks);
  int n = pick(decks, "Choose the deck you want to use");
  std::vector<std::string> load_s{LoadDeck(decks[n], true)};  //Loads all cards names from the deck into a vector
  std::vector<Card> v_card;
  LoadMyDeck(cards_database, load_s, v_card);
  Deck chosen_deck{v_card};
  chosen_deck.shuffle();
  king1.setDeck(chosen_deck);
  
  // Starting the game
  bool first = peer.value().getFirst();
  int round {0};
  for(int i=0; i<STARTING_CARDS; i++){             //Add Cards to players hand, all players need to start with 3 cards in their hand
    if( king1.getDeck().getSize() > 0){
      king1.drawCard();
    }
    king2.getHand().push_back(DEFAULT_CARD);
  }
  while(king1.getHp()>0 && king2.getHp()>0){
    round++;
    sleepForMs(WAIT_TIME);
    std::cout << '\n';
    printHugeSeparation();
    std::cout << "\nRound " << round << " - Turn of " << ( (round+first)%2==0 ? king1.getName() : king2.getName()) << '\n';
    sleepForMs(WAIT_TIME);
    
    if( (round+first) % 2==0){
      playRound(king1, king2, round, peer.value());
    }else{
      playEnemyRound(king1, king2, async_que, cards_database);
    }
  }
  // Game end and clearing up
  std::cout << "Well Done ";
  if(king1.getHp()<=0){
    std::cout << king2.getName();
  }else{
    std::cout << king1.getName();
  }
  std::cout << ", you WON!!!!!" << std::endl;
  sleepForMs(WAIT_TIME/2);
  std::cout << "END" << '\n';
  sleepForMs(WAIT_TIME);
  if(thread_io.joinable()){
    work_guard.reset();
    io_context.stop();
    thread_io.join();
    std::cout << "The program is exiting successfully" << std::endl;
  }
  #ifdef _WIN32
    system("PAUSE");
  #endif
  return 0;
}

//FUNCTIONS
inline void sleepForMs(int x){
  std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

// Loops over until user enters either y or n
// Always returns value in uppercase
char getAnswer(){
  char answer;
  do{
    std::cin >> answer;
    answer = toupper(answer);
    if(answer == 'Y' || answer == 'N'){
      return answer;
    }else{
      std::cout << "Please answer with Y or N" << '\n';
    }
  }while(true);
}
// Loops over until user enters a number between 1 and size
int pickBase(int size){
  char answer[10];
  do{
    std::cin >> answer;
    if(std::atoi(answer) > size || std::atoi(answer) < 1){
      std::cout << "Please enter a number between 1 and " << size << '\n';
    }else{
      return std::atoi(answer)-1;   //reduce by 1 to make it between 0 and size-1
    }
  }while(true);
}
// Prints the vector with all the options user can choose from, and then calls pickBase to get user response
template<typename T>
int pick(std::vector<T> &vec, std::string action){
  std::cout << action << " 1 - " << vec.size() << '\n';
  if constexpr (std::is_same<T, std::string>::value){   // String print
    printVec(vec);
  }else if constexpr (std::is_same<T, Card>::value ){   // Card print
    printVecCard(vec);
  }else{
    std::cerr << "There was an error with the pick template" << std::endl;
  }
  return pickBase(vec.size());
}

void playRound(King &k1, King &k2, int round, Peer &p){
  if( k1.getDeck().getSize() > 0){        //Check if we still have Cards in the deck
    k1.drawCard();                        //If so we draw a card
    std::cout << "You drawed: " << k1.getHand().back().getName() << " " << k1.getHand().back().getLevel() << "* \n\n";
    p.sendData("Draw ");
  }else{
    p.sendData("No Draw");
  }
  printAllKingdoms(k1, k2);
  if(k1.getHand().size()!=0){
    std::cout << "Do you want to summon? (Y/N) ";
    if(getAnswer() == 'Y'){
      k1.summon( pick(k1.getHand(), "Choose a card to summon") );
      std::cout << "You summoned: " << k1.getLastSummon() << '\n';
      p.sendData("Summon " + k1.getLastSummon());
      if(round!=1){
        std::cout << '\n';
        printAllKingdoms(k1, k2);
      }
    }else{
      p.sendData("No Summon");
    }
  }else{
    std::cout << "You can't summon, you have 0 cards in hand" << '\n';
    p.sendData("No Summon");
  }
  if(round!=1 && k1.getEvocationZone().size()!=0){
    std::cout << "Do you want to attack? (Y/N) ";
    if(getAnswer() == 'Y'){
      int pos_atk = pick(k1.getEvocationZone(), "Choose subservient to attack with ");
      if(k2.isEmpty()){
        p.sendData(std::to_string(pos_atk) + " to " + "null");
        std::cout << "You made a Direct Attack and dealt " << k1.getAtk(pos_atk) << " damage\n";
        k2.reduceHp(k1.getAtk(pos_atk));
      }else{
        int pos_def = pick(k2.getEvocationZone(), "Choose who to attack");
        p.sendData(std::to_string(pos_atk) + " to " + std::to_string(pos_def));
        std::cout << "You attacked and ";
        int diff = k1.getAtk(pos_atk)-k2.getDef(pos_def);
        if(diff > 0){
          std::cout << "dealt " << diff << " damage\n";
          k2.reduceHp(diff);
          k2.destroyCard(pos_def);
        }else if(diff < 0){
          std::cout << "took " << -diff << " damage\n";
          k1.reduceHp(-diff);
          k1.destroyCard(pos_atk);
        }else{ // if diff is 0 do nothing
          std::cout << "nothing happened because enemy attack is equal to yours\n";
        }    
      }
    }else{
      p.sendData("No Attack");
    }
  }else{
    std::cout << "You can't attack\n";
    p.sendData("No Attack");
  }
  sleepForMs(WAIT_TIME);
  std::cout << '\n';
}

void playEnemyRound(King &k1, King &k2, AsyncQueue& async_que, std::map<std::string, Card> cards_database){
  // Drawing Phase
  std::string s = async_que.getValue();
  if(s.substr(0,2) == "No"){
    std::cout << "Enemy didn't draw \n\n";
  }else{
    std::cout << "Enemy drawed \n\n";
    k2.getHand().push_back(DEFAULT_CARD);
  }
  printAllKingdoms(k1, k2);
  s = async_que.getValue();
  // Summoning Phase
  if(s.substr(0,2) == "No"){
    std::cout << "Enemy didn't summon\n";
  }else{
    std::cout << "Enemy summoned " << s.substr(7) << '\n';
    if(cards_database.contains(s.substr(7))){
      k2.getEvocationZone().push_back(cards_database[s.substr(7)] );
    }else{
      std::cerr << "[WARNING] Enemy tried to summon unkown card" << std::endl;
    }
  }
  // Attack Phase
  s = async_que.getValue();
  if(s.substr(0,2) == "No"){
    std::cout << "Enemy didn't attack \n";
  }else{
    int pos_atk = stoi(s.substr(0,1));  // First byte is the index of the attacking card
    s = s.substr(5);  // The four byte in the middle are text that can be ingnored
    if(s == "null"){  // If the remaining text is null the opponent made a direct attack
      std::cout << "You took a Direct Attack and lost " << k2.getAtk(pos_atk) << " HP \n";
      k1.reduceHp(k2.getAtk(pos_atk));
    }else{
      int pos_def = stoi(s);
      std::cout << k2.getName() << " attacked and you ";
      int diff = k1.getDef(pos_def)-k2.getAtk(pos_atk);
      if(diff > 0){
        std::cout << "dealt " << diff << " damage\n";
        k2.reduceHp(diff);
        k2.destroyCard(pos_atk);
      }else if(diff < 0){
        std::cout << "took " << -diff << " damage\n";
        k1.reduceHp(-diff);
        k1.destroyCard(pos_def);
      }else{
        std::cout << "nothing happened because enemy attack is equal to yours\n";
      }
    }
  }
  sleepForMs(WAIT_TIME);
  std::cout << '\n';
}

void printAllKingdoms(King &k1, King &k2){
  sleepForMs(WAIT_TIME);
  std::cout << std::left;
  printBorderSeparation();
  std::cout << k2.getName() << " HP " << k2.getHp() << " | " << k2.getHand().size() << " cards in hand" << std::endl;
  printSeparation();
  sleepForMs(WAIT_TIME/4);
  k2.printKingdom();
  sleepForMs(WAIT_TIME);
  printMidSeparation();
  k1.printKingdom();
  sleepForMs(WAIT_TIME);
  printSeparation();
  std::cout << k1.getName() << " HP " << k1.getHp() << std::endl;
  printBorderSeparation();
  sleepForMs(WAIT_TIME/2);
  std::cout << "\nCards in your hand" << std::endl;
  printVecCard(k1.getHand());
  std::cout << std::resetiosflags(std::ios_base::adjustfield);
}