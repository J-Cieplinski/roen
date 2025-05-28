#ifndef ROEN_SCENE_GRAPH_HPP
#define ROEN_SCENE_GRAPH_HPP

#include <entt/fwd.hpp>

#include <unordered_set>

namespace roen
{

struct Hierarchy
{
    entt::entity parent{entt::null};
    std::unordered_set<entt::entity> children;
};

}  // namespace roen

#endif  // ROEN_SCENE_GRAPH_HPP
