project(data_structure_ut)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MapNodeTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PriorityQueueTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemsContainerTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector2Tests.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    roen)

add_test(DataStructureTests ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${MODULES_DIR}/roen/include)