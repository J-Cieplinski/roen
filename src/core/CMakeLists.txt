target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/core/Assets.hpp
    ${INCLUDE_DIR}/core/AssetManager.hpp
    ${INCLUDE_DIR}/core/DefaultAsset.hpp
    ${INCLUDE_DIR}/core/Input.hpp
    ${INCLUDE_DIR}/core/KeyCodes.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Input.cpp
)