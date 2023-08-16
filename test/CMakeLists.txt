project(mdspan_formatter_test)

find_package(fmt REQUIRED)
find_package(ut REQUIRED)

add_executable(mdspan_formatter_test main.cpp)
target_link_libraries(mdspan_formatter_test PRIVATE mdspan_formatter fmt::fmt Boost::ut)