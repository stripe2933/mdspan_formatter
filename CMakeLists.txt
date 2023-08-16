cmake_minimum_required(VERSION 3.6)
project(mdspan_formatter
    DESCRIPTION "Make std::mdspan formattable by std::format (or fmt::format)."
    HOMEPAGE_URL "https://github.com/stripe2933/mdspan_formatter"
    LANGUAGES CXX)

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
target_link_libraries(${PROJECT_NAME} INTERFACE std::mdspan)

# Determine if the project is built as a main project or a subproject (using add_subdirectory).
# If main project, build test.
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(BUILD_TEST ON)
else()
    set(BUILD_TEST OFF)
endif()

if (BUILD_TEST)
    add_subdirectory(test)
endif()