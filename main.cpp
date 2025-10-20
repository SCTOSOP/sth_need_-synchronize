/*
 * @Author: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @Date: 2025-10-20 11:08:25
 * @LastEditors: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @LastEditTime: 2025-10-20 12:58:16
 * @FilePath: /pipeline_test/main.cpp
 * @Description:
 */

#include "PipeLine.h"
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>

void p1(PipeLineSolution<int> &data) {
  int random_int = 0;
  while (true) {
    random_int = rand();
    data.setData(std::make_shared<int>(random_int));
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
}

void p2(PipeLineSolution<int> &data) {
  while (true) {
    std::shared_ptr<int> int_data = data.waitData();
    std::cout << "get data " << *int_data << std::endl;
  }
}

void Stop(int signo) { exit(0); }

int main(void) {
  signal(SIGINT, Stop);

  PipeLineSolution<int> data;
  std::thread t1(p1, std::ref(data));
  std::thread t2(p2, std::ref(data));

  t1.join();
  t2.join();
}