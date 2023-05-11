project(mdspan_formatter_test)

add_executable(mdspan_formatter_test main.cpp)

find_package(GTest REQUIRED)
target_link_libraries(mdspan_formatter_test PRIVATE mdspan_formatter GTest::GTest)