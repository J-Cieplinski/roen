#include <algorithms/search/Pathfinding.hpp>
#include <data_structure/Graph.hpp>
#include <data_structure/Vector2.hpp>

#include <gtest/gtest.h>

namespace roen::algorithms
{

namespace
{

constexpr data_structure::Vector2f DUMMY_SIZE {16, 16};
constexpr std::uint32_t DUMMY_COST {0};

} //

class PathfindingTests : public testing::Test
{
protected:
    const data_structure::MapNode startNode {{0,0}, DUMMY_SIZE, DUMMY_COST};
    const data_structure::MapNode middleNode{{16, 16}, DUMMY_SIZE, DUMMY_COST};
    const data_structure::MapNode endNode {{32,32}, DUMMY_SIZE, DUMMY_COST};
};

TEST_F(PathfindingTests, getNodeOrderFromPath_ShouldReturnCorrectPath)
{
    const std::unordered_map<data_structure::MapNode, data_structure::MapNode> path {
            {middleNode, startNode},
            {endNode, middleNode}
    };

    const std::vector<data_structure::MapNode> EXPECTED_ORDER {startNode, middleNode, endNode};

    EXPECT_EQ(getNodeOrderFromPath(startNode, endNode, path), EXPECTED_ORDER);
}

TEST_F(PathfindingTests, getNodeOrderFromPath_IfGoalDoesNotBelongToPathItShouldReturnEmptyVector)
{
    const std::unordered_map<data_structure::MapNode, data_structure::MapNode> path {
            {middleNode, startNode},
    };

    const std::vector<data_structure::MapNode> EXPECTED_ORDER {};

    EXPECT_EQ(getNodeOrderFromPath(startNode, endNode, path), EXPECTED_ORDER);
}

TEST_F(PathfindingTests, a_star_ShouldFindCorrectPath)
{
    /*
     * startNode <---> middleNode <---> endNode
     */
    data_structure::Graph<data_structure::MapNode> graph;
    graph.addNode(startNode, {middleNode});
    graph.addNode(middleNode, {startNode, endNode});
    graph.addNode(endNode, {middleNode});

    const std::unordered_map<data_structure::MapNode, data_structure::MapNode> EXPECTED_PATH {
            {startNode, startNode},
            {middleNode, startNode},
            {endNode, middleNode}
    };

    const std::unordered_map<data_structure::MapNode, data_structure::MapNode> EXPECTED_REVERSE_PATH {
            {endNode, endNode},
            {middleNode, endNode},
            {startNode, middleNode},
    };

    EXPECT_EQ(a_star(startNode, endNode, graph, manhattanDistance), EXPECTED_PATH);
    EXPECT_EQ(a_star(endNode, startNode, graph, manhattanDistance), EXPECTED_REVERSE_PATH);
}

TEST_F(PathfindingTests, a_star_ShouldReturnPathSoFarIfPathCannotBeFound)
{
    /*
     * startNode <---> middleNode <--- endNode
     */
    data_structure::Graph<data_structure::MapNode> graph;
    graph.addNode(startNode, {middleNode});
    graph.addNode(middleNode, {startNode});
    graph.addNode(endNode, {middleNode});

    const std::unordered_map<data_structure::MapNode, data_structure::MapNode> EXPECTED_INCOMPLETE_PATH {
            {startNode, startNode},
            {middleNode, startNode},
    };

    EXPECT_EQ(a_star(startNode, endNode, graph, manhattanDistance), EXPECTED_INCOMPLETE_PATH);
}

} // roen::algorithms
