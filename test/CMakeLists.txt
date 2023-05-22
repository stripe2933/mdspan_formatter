project(mdspan_formatter_test)

enable_testing()

find_package(GTest REQUIRED)
find_package(fmt REQUIRED)

add_executable(mdspan_formatter_test
        main.cpp
        std_layout_right_test.cpp std_layout_left_test.cpp
        fmt_layout_right_test.cpp fmt_layout_left_test.cpp)
target_link_libraries(mdspan_formatter_test PRIVATE mdspan_formatter GTest::GTest fmt::fmt)

gtest_discover_tests(mdspan_formatter_test)