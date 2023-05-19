cmake_minimum_required(VERSION 3.25)
project(mdspan_formatter)

set(BUILD_TEST ON) # ON -> Build test (requires GoogleTest and fmt)

add_library(mdspan_formatter INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # To use std::format in Clang, -fexperimental-library compile option and
    # libc++experimental.a library must be explicitly linked.
    target_compile_options(${PROJECT_NAME} INTERFACE -fexperimental-library)
    target_link_directories(${PROJECT_NAME} INTERFACE /opt/homebrew/opt/llvm/lib/c++) # Your libc++ lib directory
    target_link_libraries(${PROJECT_NAME} INTERFACE c++experimental)
endif()

find_package(mdspan REQUIRED)
find_package(fmt REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE std::mdspan fmt::fmt)

if (BUILD_TEST)
    add_subdirectory(test)
endif()