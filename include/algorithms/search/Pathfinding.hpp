#ifndef ROEN_ALGORITHMS_SEARCH_PATHFINDING_HPP
#define ROEN_ALGORITHMS_SEARCH_PATHFINDING_HPP

#include <data_structure/MapNode.hpp>
#include <data_structure/Graph.hpp>

#include <functional>
#include <vector>
#include <unordered_map>

namespace roen::algorithms
{

std::unordered_map<data_structure::MapNode, data_structure::MapNode> a_star(
        const data_structure::MapNode& start,
        const data_structure::MapNode& goal,
        const data_structure::Graph<data_structure::MapNode>& graph,
        const std::function<float(const data_structure::MapNode& nodeA, const data_structure::MapNode& nodeB)>& heuristic);

std::vector<data_structure::MapNode> getNodeOrderFromPath(
        const data_structure::MapNode& start,
        const data_structure::MapNode& goal,
        const std::unordered_map<data_structure::MapNode, data_structure::MapNode>& path);

float manhattanDistance(const data_structure::MapNode& nodeA, const data_structure::MapNode& nodeB);

} // roen::algorithms

#endif //ROEN_ALGORITHMS_SEARCH_PATHFINDING_HPP
