target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/log/Logger.hpp
    ${INCLUDE_DIR}/log/formatters/entity.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
)