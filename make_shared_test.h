/*
 * @Author: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @Date: 2025-10-26 09:40:53
 * @LastEditors: 在百慕大钓鱼的人 mayuzhuonor@proton.me
 * @LastEditTime: 2025-10-26 09:46:36
 * @FilePath: /pipeline_test/make_shared_test.h
 * @Description: MAKE_SHARED_TEST_H
 */
#ifndef MAKE_SHARED_TEST_H
#define MAKE_SHARED_TEST_H

#include <iostream>
#include <ostream>
#include <string>
class make_shared_test_class {
public:
  make_shared_test_class(std::string info) : info(std::move(info)) {
    std::cout << this->info << " make_shared_test_class" << std::endl;
  }
  ~make_shared_test_class() {
    std::cout << this->info << " ~make_shared_test_class" << std::endl;
  }

protected:
  std::string info;
};

#endif // MAKE_SHARED_TEST_H