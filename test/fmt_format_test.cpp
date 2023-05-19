#include <gtest/gtest.h>

#define MDSPAN_FORMATTER_USE_FMT
#include <mdspan_formatter.hpp>

using namespace Kokkos;

TEST(MDSPAN_FMT_FORMATTER_TEST, FIXED_SIZE_INTS_TEST){
    auto nums = std::array { 1, 2, 3, 4, 5, 6 };

    auto nums1d = mdspan<int, extents<std::size_t, 6>> { nums.data() };
    EXPECT_EQ("[1, 2, 3, 4, 5, 6]", fmt::format("{}", nums1d));

    auto nums2x3 = mdspan<int, extents<std::ptrdiff_t, 2, 3>> { nums.data() };
    EXPECT_EQ("[[1, 2, 3],\n"
              " [4, 5, 6]]", fmt::format("{}", nums2x3));

    auto nums6x1 = mdspan<int, extents<int, 6, 1>> { nums.data() };
    EXPECT_EQ("[[1],\n"
              " [2],\n"
              " [3],\n"
              " [4],\n"
              " [5],\n"
              " [6]]", fmt::format("{}", nums6x1));
}

TEST(MDSPAN_FMT_FORMATTER_TEST, FIXED_SIZE_FLOATS_TEST){
    auto nums = std::array {
        1.f, 2.f, 3.f, 4.f,
        5.f, 6.f, 7.f, 8.f,
        9.f, 10.f, 11.f, 12.f,
        13.f, 14.f, 15.f, 16.f
    };

    auto nums1x16 = mdspan<float, extents<std::size_t, 1, 16>> { nums.data() };
    EXPECT_EQ("[[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]]", fmt::format("{}", nums1x16));

    auto nums4x4 = mdspan<float, extents<std::size_t, 4, 4>> { nums.data() };
    EXPECT_EQ("[[ 1.0,  2.0,  3.0,  4.0],\n"
              " [ 5.0,  6.0,  7.0,  8.0],\n"
              " [ 9.0, 10.0, 11.0, 12.0],\n"
              " [13.0, 14.0, 15.0, 16.0]]", fmt::format("{:: >4.1f}", nums4x4));

    auto nums2x8 = mdspan<float, extents<std::ptrdiff_t, 8, 2>> { nums.data() };
    EXPECT_EQ("[[1.00, 2.00],\n"
              " [3.00, 4.00],\n"
              " [5.00, 6.00],\n"
              " [7.00, 8.00],\n"
              " [9.00, 10.0],\n"
              " [11.0, 12.0],\n"
              " [13.0, 14.0],\n"
              " [15.0, 16.0]]", fmt::format("{::0<4.1f}", nums2x8));

    auto nums2x2x4 = mdspan<float, extents<std::ptrdiff_t, 2, 2, 4>> { nums.data() };
    EXPECT_EQ("[[[  1,   2,   3,   4],\n"
              "  [  5,   6,   7,   8]],\n"
              " [[  9,  10,  11,  12],\n"
              "  [ 13,  14,  15,  16]]]", fmt::format("{:: >3}", nums2x2x4));
}

TEST(MDSPAN_FMT_FORMATTER_TEST, FIXED_SIZE_CHARS_TEST){
    auto magic_square = std::array {
        'S', 'A', 'T', 'O', 'R',
        'A', 'R', 'E', 'P', 'O',
        'T', 'E', 'N', 'E', 'T',
        'O', 'P', 'E', 'R', 'A',
        'R', 'O', 'T', 'A', 'S'
    };

    auto square5x5 = mdspan<char, extents<std::ptrdiff_t, 5, 5>> { magic_square.data() };
//    EXPECT_EQ("[SATOR,\n"
//              " AREPO,\n"
//              " TENET,\n"
//              " OPERA,\n"
//              " ROTAS]", fmt::format("{:s}", square5x5));
    EXPECT_EQ("[['S', 'A', 'T', 'O', 'R'],\n"
              " ['A', 'R', 'E', 'P', 'O'],\n"
              " ['T', 'E', 'N', 'E', 'T'],\n"
              " ['O', 'P', 'E', 'R', 'A'],\n"
              " ['R', 'O', 'T', 'A', 'S']]", fmt::format("{::?}", square5x5));
}

TEST(MDSPAN_FMT_FORMATTER_TEST, DYNAMIC_SIZE_INTS_TEST){
    auto nums = std::array { 1, 2, 3, 4, 5, 6 };

    auto nums1d = mdspan { nums.data(), 6 };
    EXPECT_EQ("[1, 2, 3, 4, 5, 6]", fmt::format("{}", nums1d));

    auto nums2x3 = mdspan<int, extents<std::ptrdiff_t, std::dynamic_extent, 3>> { nums.data(), 2 };
    EXPECT_EQ("[[1, 2, 3],\n"
              " [4, 5, 6]]", fmt::format("{}", nums2x3));

    auto nums6x1 = mdspan<int, dextents<int, 2>> { nums.data(), 6, 1 };
    EXPECT_EQ("[[1],\n"
              " [2],\n"
              " [3],\n"
              " [4],\n"
              " [5],\n"
              " [6]]", fmt::format("{}", nums6x1));
}

TEST(MDSPAN_FMT_FORMATTER_TEST, DYNAMIC_SIZE_FLOATS_TEST){
    auto nums = std::array {
            1.f, 2.f, 3.f, 4.f,
            5.f, 6.f, 7.f, 8.f,
            9.f, 10.f, 11.f, 12.f,
            13.f, 14.f, 15.f, 16.f
    };

    auto nums1x16 = mdspan<float, extents<std::size_t, 1, std::dynamic_extent>> { nums.data(), 16 };
    EXPECT_EQ("[[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]]", fmt::format("{}", nums1x16));

    auto nums4x4 = mdspan<float, extents<std::size_t, std::dynamic_extent, 4>> { nums.data(), 4 };
    EXPECT_EQ("[[ 1.0,  2.0,  3.0,  4.0],\n"
              " [ 5.0,  6.0,  7.0,  8.0],\n"
              " [ 9.0, 10.0, 11.0, 12.0],\n"
              " [13.0, 14.0, 15.0, 16.0]]", fmt::format("{:: >4.1f}", nums4x4));

    auto nums8x2 = mdspan<float, dextents<std::ptrdiff_t, 2>> { nums.data(), 8, 2 };
    EXPECT_EQ("[[1.00, 2.00],\n"
              " [3.00, 4.00],\n"
              " [5.00, 6.00],\n"
              " [7.00, 8.00],\n"
              " [9.00, 10.0],\n"
              " [11.0, 12.0],\n"
              " [13.0, 14.0],\n"
              " [15.0, 16.0]]", fmt::format("{::0<4.1f}", nums8x2));

    auto nums2x2x4 = mdspan { nums.data(), 2, 2, 4 };
    EXPECT_EQ("[[[  1,   2,   3,   4],\n"
              "  [  5,   6,   7,   8]],\n"
              " [[  9,  10,  11,  12],\n"
              "  [ 13,  14,  15,  16]]]", fmt::format("{:: >3}", nums2x2x4));
}

TEST(MDSPAN_FMT_FORMATTER_TEST, DYNAMIC_SIZE_CHARS_TEST){
    auto magic_square = std::array {
            'S', 'A', 'T', 'O', 'R',
            'A', 'R', 'E', 'P', 'O',
            'T', 'E', 'N', 'E', 'T',
            'O', 'P', 'E', 'R', 'A',
            'R', 'O', 'T', 'A', 'S'
    };

    auto square5x5 = mdspan { magic_square.data(), 5, 5 };
//    EXPECT_EQ("[SATOR,\n"
//              " AREPO,\n"
//              " TENET,\n"
//              " OPERA,\n"
//              " ROTAS]", fmt::format("{:s}", square5x5));
    EXPECT_EQ("[['S', 'A', 'T', 'O', 'R'],\n"
              " ['A', 'R', 'E', 'P', 'O'],\n"
              " ['T', 'E', 'N', 'E', 'T'],\n"
              " ['O', 'P', 'E', 'R', 'A'],\n"
              " ['R', 'O', 'T', 'A', 'S']]", fmt::format("{::?}", square5x5));
}