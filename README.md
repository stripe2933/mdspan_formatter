# mdspan_formatter
Make `std::mdspan` (multi-dimensional span; currently [`Kokkos::mdspan`](https://github.com/kokkos/mdspan)) formattable using `std::format`.

## Usage

```c++
#include <iostream>

#include "mdspan_formatter.hpp"

template <typename... T>
void print(std::format_string<T...> fmt, T &&...args){
    std::cout << std::format(fmt, std::forward<T>(args)...);
}

int main(int, char**) {
    using namespace Kokkos;

    auto nums = std::array { 1.f, 2.f, 3.f, 4.f };
    print("{}\n", mdspan { nums.data(), 4, 1 });
    /**
     * Output:
     * 
     * [[1],
     *  [2],
     *  [3],
     *  [4]]
    */

    print("{:: >4.1f}\n", mdspan<float, extents<std::size_t, 2, 2>> { nums.data() });
    /**
     * Output:
     * 
     * [[ 1.0,  2.0],
     *  [ 3.0,  4.0]]
    */

    auto magic_square = std::array {
        'S', 'A', 'T', 'O', 'R',
        'A', 'R', 'E', 'P', 'O',
        'T', 'E', 'N', 'E', 'T',
        'O', 'P', 'E', 'R', 'A',
        'R', 'O', 'T', 'A', 'S'
    };
    print("{:s}\n", mdspan { magic_square.data(), 5, 5 });
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

It supports C++23's range formatting specifier and left spacing for rank ≥ 2 span.
For fixed size mdspan (whose extent is known at compile time), formatting is optimized using `constexpr` declared variables.

It is tested with [Google test](https://github.com/google/googletest/issues), but not robust. I appreciate your contribution.

Note that it only supports `mdspan` with `layout_right` policy and `default_accessor` accessor.

# How to use

Since it depends on range formatting and multidimensional subscript operator, you need at least C++23 compatible compiler.
I build the library using Clang 16.0.1 and libc++13. GCC and MSVC are not tested yet.

First, you need [Kokkos's mdspan](https://github.com/kokkos/mdspan), which is reference implementation of `std::mdspan` because none of compiler
implemented mdspan at now.

For use formatter, copy [the header file](https://github.com/stripe2933/mdspan_formatter/blob/main/include/mdspan_formatter.hpp).
You can use the library as header only file.

If you want to build the test, download the repository and build CMake project. Note that Google Test and Kokkos mdspan library should be found using
`find_package` command.

## TODO

- Support `layout_left` policy.
- More test.
