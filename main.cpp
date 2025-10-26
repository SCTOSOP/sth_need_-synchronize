/*
 * @Author: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @Date: 2025-10-20 11:08:25
 * @LastEditors: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @LastEditTime: 2025-10-26 09:45:51
 * @FilePath: /pipeline_test/main.cpp
 * @Description:
 */

#include "PipeLine.h"
#include "make_shared_test.h"
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>

#include "unistd.h"

static bool p1_stop_signal = false;
static bool p2_stop_signal = false;

void p1(PipeLineSolution<int> &data) {
  int random_int = 0;
  while (!p1_stop_signal) {
    random_int = rand();
    data.setData(std::make_shared<int>(random_int));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  std::cerr << "p1 back" << std::endl;
}

void p2(PipeLineSolution<int> &data) {
  while (!p2_stop_signal) {
    std::shared_ptr<int> int_data = data.tryGetData();
    if (int_data) {
      std::cout << "get data " << *int_data << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cerr << "p2 back" << std::endl;
}

void Stop(int signo) { exit(0); }

void StopStepByStep(int signo) {
  static int stop_count = 1;

  if (stop_count == 1) {
    p1_stop_signal = true;
    stop_count++;
  } else if (stop_count == 2) {
    p2_stop_signal = true;
  }
}

int main(void) {
  std::shared_ptr<make_shared_test_class> A =
      std::make_shared<make_shared_test_class>("1");
  A = std::make_shared<make_shared_test_class>("2");

  signal(SIGINT, StopStepByStep);
  int num_of_core = sysconf(_SC_NPROCESSORS_CONF);
  std::cout << "Totally " << num_of_core << " cores" << std::endl;

  PipeLineSolution<int> data;
  std::thread t1(p1, std::ref(data));
  std::thread t2(p2, std::ref(data));

  t1.join();
  t2.join();
}