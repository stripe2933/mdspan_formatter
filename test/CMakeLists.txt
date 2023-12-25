project(mdspan_formatter_test)

include(FetchContent)

# fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
    FIND_PACKAGE_ARGS NAMES fmt
)
FetchContent_MakeAvailable(fmt)

# Boost.ut
FetchContent_Declare(
    ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG v2.0.0
    FIND_PACKAGE_ARGS NAMES ut
)
FetchContent_MakeAvailable(ut)
target_compile_definitions(ut INTERFACE BOOST_UT_DISABLE_MODULE)

add_executable(mdspan_formatter_test main.cpp)
target_link_libraries(mdspan_formatter_test
    PRIVATE
        mdspan_formatter
        fmt::fmt Boost::ut
)

include(CTest)
enable_testing()

add_test(add_test mdspan_formatter_test)