cmake_minimum_required(VERSION 3.29)

project(
    roen
    VERSION 0.5
    LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.11.3
    OPTIONS
        "JSON_Install ON"
        "JSON_BuildTests OFF"
)

CPMAddPackage(
    NAME EnTT
    GITHUB_REPOSITORY skypjack/entt
    VERSION 3.14.0
)

CPMAddPackage(
    NAME raylib
    GITHUB_REPOSITORY raysan5/raylib
    GIT_TAG 5.5
)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    VERSION 1.15.0
    OPTIONS
        "SPDLOG_INSTALL TRUE"
)

CPMAddPackage(
    NAME tileson
    GITHUB_REPOSITORY J-Cieplinski/tileson
    GIT_TAG 1.0
)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

if(raylib_ADDED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${raylib_SOURCE_DIR}/src)
endif()

if(tileson_ADDED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${tileson_SOURCE_DIR})
endif()

find_package(Lua REQUIRED)

#To enable/disable spdlog logging
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC IS_DEBUG)
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -fsanitize=undefined -fsanitize=address")
        target_link_options(${PROJECT_NAME} BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
    endif (UNIX)
endif ()

if(UNIX)
    add_compile_definitions(-DUSE_WAYLAND=ON)
endif (UNIX)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${INCLUDE_DIR}/Utils.hpp
    ${INCLUDE_DIR}/interfaces/IAsset.hpp
    ${INCLUDE_DIR}/interfaces/IScene.hpp
    ${INCLUDE_DIR}/interfaces/ISystem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EntryPoint.hpp
)

add_subdirectory(dependencies)
add_subdirectory(src)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/roenpch.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    EnTT
    ${LUA_LIBRARIES}
    nlohmann_json::nlohmann_json
    spdlog::spdlog)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

packageProject(
    NAME ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    NAMESPACE ${PROJECT_NAME}
    BINARY_DIR ${PROJECT_BINARY_DIR}
    INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
    INCLUDE_DESTINATION include/${PROJECT_NAME}
    COMPATIBILITY SameMajorVersion
    DEPENDENCIES "nlohmann_json 3.11.3;EnTT;raylib;spdlog"
)