target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/manager/AssetManager.hpp
    ${INCLUDE_DIR}/manager/GameSceneManager.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GameSceneManager.cpp
)