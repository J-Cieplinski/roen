target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/interfaces/IAsset.hpp
    ${INCLUDE_DIR}/interfaces/ISystem.hpp
    ${INCLUDE_DIR}/interfaces/Scene.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp
)