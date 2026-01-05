#include "async_queue.hpp"

void AsyncQueue::setValue(std::string s){
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.emplace(s);
  cv_.notify_one();
}
    
std::string AsyncQueue::getValue(){
  std::unique_lock<std::mutex> lock(mutex_);
  cv_.wait(lock, [this]() { return !queue_.empty(); });
  std::string s = queue_.front();
  queue_.pop();
  return s;
}