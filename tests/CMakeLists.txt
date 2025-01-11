cmake_minimum_required(VERSION 3.27)

project(roen_tests LANGUAGES CXX)

option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:google/googletest@1.15.2")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")

if(TEST_INSTALLED_VERSION)
    find_package(roen REQUIRED)
else()
    CPMAddPackage(NAME roen SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

add_executable(${PROJECT_NAME})

add_subdirectory(ut)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    roen::roen
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(roen PUBLIC -Wall -Wpedantic -Wextra -Werror)
    elseif(MSVC)
        target_compile_options(roen PUBLIC /W4 /WX)
        target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
    endif()
endif()

enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)