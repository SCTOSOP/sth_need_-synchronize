/*
 * @Author: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @Date: 2025-10-20 11:08:33
 * @LastEditors: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @LastEditTime: 2025-10-20 12:49:13
 * @FilePath: /pipeline_test/PipeLine.h
 * @Description:
 */

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>

template <class T> class PipeLineSolution {
public:
  PipeLineSolution() : data(nullptr) {}
  explicit PipeLineSolution(std::shared_ptr<T> d) : data(std::move(d)) {}

  std::shared_ptr<T> tryGetData() {
    std::lock_guard<std::mutex> lock(this->mutex);
    if (!data) {
      return nullptr;
    }
    return std::move(this->data);
  }

  std::shared_ptr<T> waitData() {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->condition_variable.wait(
        lock, [this]() { return static_cast<bool>(this->data); });
    return std::move(this->data);
  }

  void setData(std::shared_ptr<T> d) {
    {
      std::lock_guard<std::mutex> lock(this->mutex);
      this->data = std::move(d);
    }
    this->condition_variable.notify_one();
  }

protected:
  std::shared_ptr<T> data;
  std::mutex mutex;
  std::condition_variable condition_variable;
};