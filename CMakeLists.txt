project(roen)

add_library(${PROJECT_NAME} SHARED)

find_package(EnTT CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

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

add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME} PUBLIC
    EnTT::EnTT
    ${LUA_LIBRARIES}
    nlohmann_json::nlohmann_json
    sol2
    spdlog::spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${DEPENDENCIES_DIR} ${LUA_INCLUDE_DIR})