#ifndef ASYNC_QUEUE_HPP
#define ASYNC_QUEUE_HPP

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

/**
 * @brief 
 * AsyncQueue should only be consumed by one thread at a time, using getValue() asynchronously can lead to race condition or incorrect values
 */
class AsyncQueue{
  private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

  public:
    void setValue(std::string s);
    std::string getValue();
};

#endif