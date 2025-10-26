#include <cstring>
#include <fstream>
#include <iostream>
#include <tuple>
#include <type_traits>

// 简化的 DataQuickSave 类
class DataQuickSave {
public:
  // 构造函数，接受文件路径
  explicit DataQuickSave(const std::string &file_path)
      : file_path_(file_path), is_header_written_(false) {}

  // 写入数据
  template <typename... Types> void write(Types... values) {
    // 如果文件没有成功打开，直接返回
    if (!okay()) {
      std::cerr << "Failed to open file for writing." << std::endl;
      return;
    }

    // 打开文件并保持文件流
    std::ofstream fs(file_path_,
                     std::ios::binary | std::ios::out | std::ios::app);
    if (!fs) {
      std::cerr << "Failed to open file." << std::endl;
      return;
    }

    // 写入文件头（仅写一次）
    if (!is_header_written_) {
      write_header(fs);
      is_header_written_ = true; // 标记文件头已写入
    }

    // 写入实际数据
    write_data(fs, values...);

    fs.close();
  }

  // 设置数据类型并存储
  template <typename... Types> void set_data_type(Types... types) {
    data_types_ = std::make_tuple(types...); // 存储类型信息
  }

  // 判断文件是否成功打开
  bool okay() {
    std::ifstream fs(file_path_);
    return fs.good(); // 检查文件是否存在且可访问
  }

  // 关闭文件
  void close() {
    // 这里实际上不需要做任何操作，文件会在写入后自动关闭
    std::cout << "File closed." << std::endl;
  }

private:
  std::string file_path_;
  bool is_header_written_;
  std::tuple<> data_types_;

  // 写入文件头，类型名称
  void write_header(std::ofstream &fs) {
    auto write_string = [&fs](const char *str) {
      fs.write(str, std::strlen(str) + 1); // 添加'\0'结束符
    };

    // 对元组中的每个类型调用对应的标识符写入
    write_header_impl(fs, data_types_);
  }

  // 递归写入文件头
  template <typename... Types>
  void write_header_impl(std::ofstream &fs, std::tuple<Types...> &) {
    (write_header_impl_for_type<Types>(fs), ...); // 扩展包
  }

  // 对每种类型的模板特化
  template <typename T> void write_header_impl_for_type(std::ofstream &fs) {
    write_string(type_to_string<T>().c_str()); // 获取对应的类型标识符并写入
  }

  // 转换类型到相应标识符
  template <typename T> std::string type_to_string() {
    if constexpr (std::is_same_v<T, int>) {
      return "d"; // 'd' 代表 int
    } else if constexpr (std::is_same_v<T, long long>) {
      return "ld"; // 'ld' 代表 long long
    } else if constexpr (std::is_same_v<T, float>) {
      return "f"; // 'f' 代表 float
    } else if constexpr (std::is_same_v<T, double>) {
      return "lf"; // 'lf' 代表 double
    } else if constexpr (std::is_same_v<T, char>) {
      return "c"; // 'c' 代表 char
    } else if constexpr (std::is_same_v<T, bool>) {
      return "b"; // 'b' 代表 bool
    } else if constexpr (std::is_same_v<T, short>) {
      return "s"; // 's' 代表 short
    } else if constexpr (std::is_same_v<T, unsigned int>) {
      return "u"; // 'u' 代表 unsigned int
    } else if constexpr (std::is_same_v<T, unsigned long long>) {
      return "lu"; // 'lu' 代表 unsigned long long
    } else {
      return "unknown"; // 处理未知类型
    }
  }

  // 写入实际数据
  template <typename T, typename... Types>
  void write_data(std::ofstream &fs, T value, Types... values) {
    fs.write(reinterpret_cast<const char *>(&value), sizeof(T)); // 写入数据
    write_data(fs, values...); // 递归处理剩余数据
  }

  // 递归终止条件
  void write_data(std::ofstream &fs) {}

  // 写入字符串（添加\0）
  void write_string(std::ofstream &fs, const char *str) {
    fs.write(str, std::strlen(str) + 1); // 添加'\0'结束符
  }
};

int main() {
  DataQuickSave dqs("data.dat");

  // 设置数据类型，并写入数据
  dqs.set_data_type<int, long long, double, float>(
      "first int", "second long long", "third double", "fourth float");
  dqs.write(1, 10000000000LL, 3.14159, 2.718f);

  // 如果需要关闭文件
  dqs.close();

  return 0;
}
