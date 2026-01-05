#ifndef PEER_HPP
#define PEER_HPP

#include <string>

#include <asio.hpp>
#include "async_queue.hpp"

class Peer{
private:
  asio::ip::tcp::socket socket_;
  asio::ip::tcp::acceptor acceptor_;
  asio::ip::tcp::endpoint endpoint_;
  AsyncQueue& async_queue_;
  bool first_;
  std::string state_;
  char data_[257];

  void connectToPeer();
  void acceptPeer();
  void receiveData();
public:
  Peer(asio::io_context& io_context, const std::string& peer_ip, AsyncQueue& async_que, unsigned short port=33007);

  void sendData(std::string data);
  bool getFirst();
  void getState();
};

#endif