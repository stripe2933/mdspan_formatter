project(mdspan_formatter_test)

enable_testing()

find_package(GTest REQUIRED)

add_executable(mdspan_formatter_test main.cpp std_format_test.cpp fmt_format_test.cpp)
target_link_libraries(mdspan_formatter_test PRIVATE mdspan_formatter GTest::GTest)

gtest_discover_tests(mdspan_formatter_test)