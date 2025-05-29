target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/core/render/RaylibRenderer.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/RaylibRenderer.cpp
)