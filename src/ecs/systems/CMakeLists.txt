target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/LifecycleSystem.cpp
    PUBLIC
    ${INCLUDE_DIR}/ecs/systems/LifecycleSystem.hpp
)
