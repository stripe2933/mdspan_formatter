cmake_minimum_required(VERSION 3.6)
project(mdspan_formatter
    DESCRIPTION "Make std::mdspan formattable by std::format (or fmt::format)."
    HOMEPAGE_URL "https://github.com/stripe2933/mdspan_formatter"
    LANGUAGES CXX)

add_library(mdspan_formatter INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

include(FetchContent)

# Fetch mdspan
FetchContent_Declare(
    mdspan
    GIT_REPOSITORY https://github.com/kokkos/mdspan.git
    GIT_TAG stable
)
FetchContent_MakeAvailable(mdspan)

target_link_libraries(${PROJECT_NAME} INTERFACE std::mdspan)

# Set MDSPAN_FORMATTER_BUILD_TEST flag if it is unset and project is top-level project.
if (NOT DEFINED MDSPAN_FORMATTER_BUILD_TEST)
    set(MDSPAN_FORMATTER_BUILD_TEST (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR))
endif()

if (MDSPAN_FORMATTER_BUILD_TEST)
    add_subdirectory(test)
endif()