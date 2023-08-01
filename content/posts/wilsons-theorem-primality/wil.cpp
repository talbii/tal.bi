#include <cstdint>
#include <iostream>
#include <ratio>

#include <libdivide.h> // -I/path/to/libdivide
#include <bench.hpp>   // -I/path/to/<https://github.com/talbii/bench.hpp>

bool stock(std::uint64_t n) {
    std::uint64_t fact = 1u;
    for (auto i = 2u; i < n; ++i)
        fact = (fact * i) % n;
    return fact == (n - 1);
}

bool libdivide_opt(std::uint64_t n) {
    std::uint64_t fact = 1u;
    auto divisor = libdivide::divider{n};

    for (auto i = 2u; i < n; ++i) {
        auto t = fact * i;
        fact = t - (t / divisor) * n; 
    }
    return fact == (n - 1);
}

bool stock_unroll4(std::uint64_t n) {
    std::uint64_t fact = 1u;
#pragma unroll 4
    for (auto i = 2u; i < n; ++i)
        fact = (fact * i) % n;

    return fact == (n - 1);
}

bool libdivide_unroll4(std::uint64_t n) {
    std::uint64_t fact = 1u;
    auto divisor = libdivide::divider{n}; 
#pragma unroll 4
    for (auto i = 2u; i < n; ++i) {
        auto t = fact * i;
        fact = t - (t / divisor) * n;
    }

    return fact == (n - 1);
}

bool libdivide_unrolled2(std::uint64_t n) {
      std::uint64_t fact = 1u;
      auto divisor = libdivide::divider{n}; 

      std::uint64_t i = 2u;
      for (; i < n - 1; i += 2) {
          { // first unroll
              auto t = fact * i;
              fact = t - (t / divisor) * n;
          }
          { // second unroll
              auto t = fact * (i + 1);
              fact = t - (t / divisor) * n;
          }
      }

      if (i == (n - 1)) {
          auto t = fact * i;
          fact = t - (t / divisor) * n;
      }

      return fact == (n - 1);
}

int main(void) {
    constexpr auto trials = 5;
    std::uint64_t n{};
    std::cout << "Enter n: ";
    std::cin >> n;

#define B(func) bench<std::ratio<1>>(trials, func, n)

    std::cout << "no opts:\n"
              << B(stock) << '\n';

    std::cout << "libdivide:\n"
              << B(libdivide_opt) << '\n';

    std::cout << "stock+unroll4 (clang):\n"
              << B(stock_unroll4) << '\n';

    std::cout << "libdivide+unroll4 (clang):\n"
              << B(libdivide_unroll4) << '\n';

    std::cout << "libdivide+unroll2:\n"
              << B(libdivide_unrolled2) << '\n';
}
