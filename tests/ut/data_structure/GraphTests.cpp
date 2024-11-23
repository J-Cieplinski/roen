#include <data_structure/Graph.hpp>

#include <gtest/gtest.h>

namespace roen::data_structure
{

class GraphTests : public testing::Test
{
};

TEST_F(GraphTests, addNode_ShouldAddNodeWithItsEdges)
{
    const std::unordered_map<char, std::vector<char>> EXPECTED_EDGES = {
            {'A', {'B', 'C'}}
    };

    Graph<char> graph;
    graph.addNode('A', {'B', 'C'});

    EXPECT_EQ(graph.getEdges(), EXPECTED_EDGES);
}

TEST_F(GraphTests, containsNode_ShouldReturnTrueIfNodeIsPresent)
{
    Graph<char> graph;
    graph.addNode('A', {'B', 'C'});
    graph.addNode('B', {'A', 'C'});

    EXPECT_TRUE(graph.containsNode('A'));
    EXPECT_TRUE(graph.containsNode('B'));
}

TEST_F(GraphTests, neighbors_ShouldReturnNodeNeighbours)
{
    const std::vector<char> EXPECTED_NEIGHBOURS {'B', 'C'};
    Graph<char> graph;
    graph.addNode('A', {'B', 'C'});
    graph.addNode('B', {'A', 'C'});

    EXPECT_EQ(graph.neighbors('A'), EXPECTED_NEIGHBOURS);
}

} // roen::data_structure