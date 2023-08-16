# mdspan_formatter
Make `std::mdspan` (multi-dimensional span; currently [`Kokkos::mdspan`](https://github.com/kokkos/mdspan)) formattable using `std::format` or [fmt](https://github.com/fmtlib/fmt).

## Usage

```c++
#include <iostream>

#include "mdspan_formatter.hpp"

template <typename... T>
void print(std::format_string<T...> fmt, T &&...args){
    std::cout << std::format(fmt, std::forward<T>(args)...);
}

int main(int, char**) {
    auto nums = std::array { 1.f, 2.f, 3.f, 4.f };
    print("{}\n", std::mdspan { nums.data(), 4, 1 });
    /**
     * Output:
     * 
     * [[1],
     *  [2],
     *  [3],
     *  [4]]
    */

    print("{:: >4.1f}\n", std::mdspan<float, std::extents<std::size_t, 2, 2>> { nums.data() });
    /**
     * Output:
     * 
     * [[ 1.0,  2.0],
     *  [ 3.0,  4.0]]
    */

    auto magic_square = std::string {
        "SATOR"
        "AREPO"
        "TENET"
        "OPERA"
        "ROTAS"
    };
    print("{:s}\n", std::mdspan { magic_square.data(), 5, 5 });
    /**
     * Output:
     * 
     * [SATOR,
     *  AREPO,
     *  TENET,
     *  OPERA,
     *  ROTAS]
    */

    return 0;
}
```

Or you can use the library with fmt by enable the macro `MDSPAN_FORMATTER_USE_FMT`.

```c++
#define MDSPAN_FORMATTER_USE_FMT
#include "mdspan_formatter.hpp"

int main(int, char**) {
    auto ints = std::array { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    auto int2x2x3 = std::mdspan<int, std::dextents<std::size_t, 3>, std::layout_left> { ints.data(), 2, 2, 3 };
    
    fmt::print("{}\n", int2x2x3);
    /**
     * Output:
     * 
     * [[[1, 2],
     *   [3, 4]],
     *  [[5, 6],
     *   [7, 8]],
     *  [[9, 10],
     *   [11, 12]]]
     */
    
    return 0;
}
```

It supports C++23's range formatting specifier and left spacing for rank ≥ 2 span.
For fixed size mdspan (whose extent is known at compile time), formatting is optimized using `constexpr` declared variables.

It is tested with [μt](https://github.com/boost-ext/ut).

Note that it only supports `mdspan` with `default_accessor` accessor.

# How to use

Since it depends on range formatter, you need at least C++23 compatible compiler
if you want to use the library with `std::format`. Currently only Clang with libc++16 supports it.
GCC and MSVC are not tested yet.

For use library with fmt, Clang ≥ 11.0.0 and GCC ≥ 10.2 with std ≥ c++20 condition are tested. MSVC is not tested yet.
 [Try on Compiler Explorer](https://godbolt.org/z/1sTd18Gxs).

First, you need [Kokkos's mdspan](https://github.com/kokkos/mdspan), which is reference implementation of `std::mdspan` because none of compiler
implemented mdspan at now.

For use formatter, copy [the header file](https://github.com/stripe2933/mdspan_formatter/blob/main/include/mdspan_formatter.hpp).
You can use the library as header only file.

If you want to build the test, download the repository and build CMake project. Note that μt, fmt and Kokkos mdspan library should be found using
`find_package` command.
