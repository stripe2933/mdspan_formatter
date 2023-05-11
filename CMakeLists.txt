cmake_minimum_required(VERSION 3.25)
project(mdspan_formatter)

set(BUILD_TEST ON) # ON -> Build test (requires GoogleTest)

add_library(mdspan_formatter INTERFACE)
target_include_directories(mdspan_formatter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(mdspan_formatter INTERFACE cxx_std_23)
set_target_properties(mdspan_formatter PROPERTIES LINKER_LANGUAGE CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # To use std::format in Clang, -fexperimental-library compile option and
    # libc++experimental.a library must be explicitly linked.
    target_compile_options(mdspan_formatter INTERFACE -fexperimental-library)
    target_link_directories(mdspan_formatter INTERFACE /opt/homebrew/Cellar/llvm/16.0.1/lib/c++) # Your libc++ lib directory
    target_link_libraries(mdspan_formatter INTERFACE c++experimental)
endif()

find_package(mdspan REQUIRED)
target_link_libraries(mdspan_formatter INTERFACE std::mdspan)

if (BUILD_TEST)
    add_subdirectory(test)
endif()