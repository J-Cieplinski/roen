target_sources(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIR}/data_structure/Graph.hpp
    ${INCLUDE_DIR}/data_structure/INode.hpp
    ${INCLUDE_DIR}/data_structure/MapNode.hpp
    ${INCLUDE_DIR}/data_structure/PriorityQueue.hpp
    ${INCLUDE_DIR}/data_structure/SystemsContainer.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/MapNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemsContainer.cpp
)