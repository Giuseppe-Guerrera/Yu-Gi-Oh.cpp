#include <iostream>
#include <string>
#include <cstdint>
#include <cstddef>

#include <asio.hpp>
#include "peer.hpp" 
#include "async_queue.hpp"

Peer::Peer(asio::io_context& io_context, const std::string& peer_ip, AsyncQueue& async_que, unsigned short port)
: socket_{io_context}, 
  acceptor_{io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), !peer_ip.empty() ? port : port-1)},
  endpoint_{!peer_ip.empty() ? asio::ip::tcp::endpoint(asio::ip::make_address(peer_ip), port-1) : asio::ip::tcp::endpoint()},
  async_queue_{async_que}{
    if (!peer_ip.empty()) {
      // Connect to the peer
      connectToPeer();
    } else {
      // Accept incoming connections
      acceptPeer();
    }
}

void Peer::connectToPeer() {
  socket_.async_connect(endpoint_, 
    [this](asio::error_code ec) {
      if (!ec) {
        first_ = true;
        async_queue_.setValue("Connection successful");
        std::cout << std::endl;
        receiveData();
      }else{
        std::cerr << "ERROR WHEN CONNECTING TO PEER " << ec.message() << std::endl;
      }
    });
}

void Peer::acceptPeer() {
  acceptor_.async_accept(socket_,
    [this](asio::error_code ec) {
      if (!ec) {
        first_ = false;
        async_queue_.setValue("Connection successful");
        std::cout << std::endl;
        receiveData();
      }else{
        std::cout << "ERROR WHEN ACCEPTING CONNECTION TO PEER " << ec.message() << std::endl;
      }
    });
}

void Peer::sendData(std::string data) {
  state_ = "Sending";
  uint8_t size = data.size();
  data = char(size) + data;
  asio::async_write(socket_, asio::buffer(data, data.size()),
    [this](asio::error_code ec, std::size_t length) {
      if (!ec) {

      }else{
        std::cout << "Error sending data\n" << ec.message() << std::endl;
        std::cout << "Data length is " << length << std::endl;
      }
    });
}

void Peer::receiveData() {
  state_ = "Receiving";
  asio::async_read(socket_, asio::buffer(data_, 1),
    [this](asio::error_code ec, std::size_t length) {
      if (!ec) {
        uint8_t size = data_[0];
        asio::async_read(socket_, asio::buffer(data_, size),
          [this](asio::error_code ec, std::size_t length) {
            if(!ec) {
              async_queue_.setValue(std::string(data_, length));
              receiveData();
            }else{
              std::cout << "Error receiving data\n" << ec.message() << std::endl;
            }
          });
      }else{
        std::cout << "Error receiving size of data\n" << ec.message() << std::endl;
        std::cout << "Data length is " << length << std::endl;
      }
    });
}

bool Peer::getFirst() {
  return first_;
}
void Peer::getState(){
  std::cout << "[STATE] state: " << state_ << std::endl;
}