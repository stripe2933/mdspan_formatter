# mdspan_formatter
Make `std::mdspan` (multi-dimensional span; currently [`Kokkos::mdspan`](https://github.com/kokkos/mdspan)) formattable using `std::format` or [`fmt::format`](https://github.com/fmtlib/fmt).

## Usage

```c++
#include <print>

#include "mdspan_formatter.hpp"

int main(int, char**) {
    std::array nums { 1.f, 2.f, 3.f, 4.f };
    std::println("{}", std::mdspan { nums.data(), 4, 1 });
    /**
     * Output:
     * 
     * [[1],
     *  [2],
     *  [3],
     *  [4]]
    */

    std::println("{:: >4.1f}", std::mdspan<float, std::extents<std::size_t, 2, 2>> { nums.data() });
    /**
     * Output:
     * 
     * [[ 1.0,  2.0],
     *  [ 3.0,  4.0]]
    */

    std::string magic_square =
        "SATOR"
        "AREPO"
        "TENET"
        "OPERA"
        "ROTAS";
    std::println("{:s}", std::mdspan { magic_square.data(), 5, 5 });
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

Or you can use the library with fmt by header file `mdspan_fmt_formatter.hpp`.

```c++
#include "mdspan_fmt_formatter.hpp"

int main(int, char**) {
    std::array ints { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    std::mdspan<int, std::dextents<std::size_t, 3>, std::layout_left> int2x2x3 { ints.data(), 2, 2, 3 };
    
    fmt::println("{}", int2x2x3);
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

```cmake
include(FetchContent)
FetchContent_Declare(
        mdspan_formatter
        GIT_REPOSITORY https://github.com/stripe2933/mdspan_formatter.git
        GIT_TAG main
)
FetchContent_MakeAvailable(mdspan_formatter)

# Now you can link the library like:
# target_link_libraries(${PROJECT_NAME} PRIVATE mdspan_formatter)
```

Since it depends on range formatter, you need at least C++23 compatible compiler.
if you want to use the library with `std::format`. Currently only Clang with libc++ ≥ 16 supports it (experimental support for libc++16, stable support for libc++ ≥ 17).
GCC and MSVC are not tested yet.

For use library with fmt, Clang ≥ 11.0.0 and GCC ≥ 10.2 with std ≥ c++20 condition are tested. MSVC is not tested yet.
 [Try on Compiler Explorer](https://godbolt.org/z/1sTd18Gxs).

First, you need [Kokkos's mdspan](https://github.com/kokkos/mdspan), which is reference implementation of `std::mdspan` because none of compiler
implemented mdspan at now.

If you want to build the test, download the repository and build CMake project. Note that μt, fmt and Kokkos mdspan library should be found using
`find_package` command.
