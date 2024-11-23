cmake_minimum_required(VERSION 3.29)

project(roen)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} STATIC)

find_package(Lua REQUIRED)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(ENTT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/entt/src)
set(DEPENDENCIES_INCLUDE_DIR ${DEPENDENCIES_DIR} ${ENTT_DIR})


#To enable/disable spdlog logging
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC IS_DEBUG)
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -fsanitize=undefined -fsanitize=address")
        target_link_options(${PROJECT_NAME} BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
    endif (UNIX)
endif ()

target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/Utils.hpp
    ${INCLUDE_DIR}/interfaces/IAsset.hpp
    ${INCLUDE_DIR}/interfaces/IScene.hpp
    ${INCLUDE_DIR}/interfaces/ISystem.hpp
)

add_subdirectory(dependencies)
add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME} PUBLIC
    EnTT::EnTT
    ${LUA_LIBRARIES}
    nlohmann_json::nlohmann_json
    spdlog::spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${DEPENDENCIES_INCLUDE_DIR} ${ENTT_DIR} ${LUA_INCLUDE_DIR})