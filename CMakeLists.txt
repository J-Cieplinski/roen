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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Compile definition to init library with raylib backend, hardcoded for now
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        RAYLIB_BACKEND
        $<$<CONFIG:Debug>:IS_DEBUG=1>
        $<$<AND:$<CONFIG:Debug>,$<C_COMPILER_ID:GNU>>:USE_WAYLAND=ON>
)

set(DEBUG_GNU_CONDITION $<AND:$<CONFIG:Debug>,$<C_COMPILER_ID:GNU>>)

# Compiler options for debug builds with GCC
target_compile_options(${PROJECT_NAME} PUBLIC
    $<${DEBUG_GNU_CONDITION}:-Werror>
    $<${DEBUG_GNU_CONDITION}:-fsanitize=undefined>
    $<${DEBUG_GNU_CONDITION}:-fsanitize=address>
)

# Link options for sanitizers (required when using sanitizer flags)
target_link_options(${PROJECT_NAME} PUBLIC
    $<${DEBUG_GNU_CONDITION}:-fsanitize=undefined>
    $<${DEBUG_GNU_CONDITION}:-fsanitize=address>
)

add_subdirectory(tests)

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
