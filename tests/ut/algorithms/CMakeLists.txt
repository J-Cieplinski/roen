project(algorithms_ut)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/PathfindingTests.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    roen)

add_test(AlgorithmTests ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${MODULES_DIR}/roen/include)