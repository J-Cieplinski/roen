#include <algorithms/search/Pathfinding.hpp>
#include <data_structure/PriorityQueue.hpp>

#include <log/Logger.hpp>

#include <algorithm>
#include <cmath>

namespace roen::algorithms
{

std::unordered_map<data_structure::MapNode, data_structure::MapNode> a_star(
        const data_structure::MapNode& start,
        const data_structure::MapNode& goal,
        const data_structure::Graph<data_structure::MapNode>& graph,
        const std::function<float(const data_structure::MapNode& nodeA, const data_structure::MapNode& nodeB)>& heuristic)
{
    std::unordered_map<data_structure::MapNode, data_structure::MapNode> came_from;
    std::unordered_map<data_structure::MapNode, std::uint32_t> cost_so_far;
    data_structure::PriorityQueue<data_structure::MapNode, std::uint32_t> frontier;
    frontier.insert({0, start});
    came_from[start] = start;
    cost_so_far[start] = 0;

    while(!frontier.empty())
    {
        auto current = frontier.get();
        if(current == goal)
        {
            break;
        }

        for(const auto& nextNode : graph.neighbors(current))
        {
            auto newCost = cost_so_far[current] + graph.cost(current, nextNode);
            if(not cost_so_far.contains(nextNode)
                || cost_so_far[nextNode] > newCost)
            {
                cost_so_far[nextNode] = newCost;
                came_from[nextNode] = current;

                auto priority = newCost + heuristic(nextNode, goal);
                frontier.insert({priority, nextNode});
            }
        }
    }

    return came_from;
}

std::vector<data_structure::MapNode> getNodeOrderFromPath(
        const data_structure::MapNode& start,
        const data_structure::MapNode& goal,
        const std::unordered_map<data_structure::MapNode, data_structure::MapNode>& path)
{
    std::vector<data_structure::MapNode> orderedPath {};
    auto current = goal;
    if(path.find(goal) == path.end())
    {
        SDK_WARN("Couldn't find path with goal: {0}", current.cost()); //TODO: print node coordinates
        return orderedPath;
    }

    while(current != start)
    {
        orderedPath.push_back(current);
        current = path.at(current);
    }

    orderedPath.push_back(start);
    std::ranges::reverse(orderedPath);

    return orderedPath;
}

float manhattanDistance(const data_structure::MapNode& nodeA, const data_structure::MapNode& nodeB)
{
    auto distance = nodeA - nodeB;
    return std::abs(distance.x) + std::abs(distance.y);
}

} // roen::algorithms
