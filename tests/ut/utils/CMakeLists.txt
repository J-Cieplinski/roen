project(utils_ut)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/UtilsTests.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    roen)

add_test(UtilsTests ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${DEPENDENCIES_INCLUDE_DIR})