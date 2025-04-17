add_subdirectory(algorithms)
add_subdirectory(core)
add_subdirectory(data_structure)
add_subdirectory(loader)
add_subdirectory(log)
add_subdirectory(manager)

target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/Application.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
)