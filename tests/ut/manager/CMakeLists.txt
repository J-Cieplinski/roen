project(manager_ut)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/AssetManagerTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameSceneManagerTests.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    roen)

add_test(ManagerTests ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${MODULES_DIR}/roen/include)