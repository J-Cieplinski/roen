target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/core/raylib/RaylibAudioPlayer.hpp
    ${INCLUDE_DIR}/core/raylib/RaylibRenderer.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/RaylibAudioPlayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RaylibRenderer.cpp
)