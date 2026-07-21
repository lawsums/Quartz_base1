

#include <iostream>
#include <type_traits>  // 标准库类型操作工具

int main() {
    auto x = 10;
    const int a = 3;
    decltype(x) y;
    decltype(a) b = 5;

    // 编译期判断类型属性（返回 bool 常量）
    std::cout << "decltype(x) 是 int 吗？" << std::is_same_v<decltype(x), int> << std::endl;
    std::cout << "decltype(a) 是 const int 吗？" << std::is_same_v<decltype(a), const int> << std::endl;
    std::cout << "decltype(b) 是 volatile int 吗？" << std::is_volatile_v<decltype(b)> << std::endl;
    std::cout << "decltype(y) 是指针类型吗？" << std::is_pointer_v<decltype(y)> << std::endl;

    return 0;
}
