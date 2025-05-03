cmake_minimum_required(VERSION 3.29)

project(roen)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} STATIC)

find_package(Lua REQUIRED)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/spdlog/include)
set(ENTT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/entt/src)
set(RAYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/raylib/src)
set(LUA_INCLUDE_DIR ${DEPENDENCIES_DIR}/sol2)
set(DEPENDENCIES_INCLUDE_DIR ${DEPENDENCIES_DIR} ${ENTT_DIR} ${RAYLIB_DIR} ${SPDLOG_DIR})


#To enable/disable spdlog logging
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC IS_DEBUG)
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -fsanitize=undefined -fsanitize=address")
        target_link_options(${PROJECT_NAME} BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
    endif (UNIX)
    add_subdirectory(tests)
endif ()

if(UNIX)
    add_compile_definitions(-DUSE_WAYLAND=ON)
endif (UNIX)

target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/Utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EntryPoint.hpp
)

add_subdirectory(dependencies)
add_subdirectory(src)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/roenpch.hpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    EnTT::EnTT
    ${LUA_LIBRARIES}
    nlohmann_json::nlohmann_json
    raylib
    spdlog::spdlog
    box2d)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${DEPENDENCIES_INCLUDE_DIR} ${ENTT_DIR} ${LUA_INCLUDE_DIR})