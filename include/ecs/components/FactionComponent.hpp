#ifndef ROEN_ECS_COMPONENTS_FACTION_COMPONENT_HPP
#define ROEN_ECS_COMPONENTS_FACTION_COMPONENT_HPP

#include <bitset>
#include <cstdint>

namespace roen::ecs::components
{

enum Faction : std::uint64_t
{
    NONE = 0u,
    PLAYER = 1u << 0,
    SKELETON = 1u << 1,
    GHOST = 1u << 2,
};

struct FactionComponent
{
    operator std::bitset<8>&()
    {
        return factionMask;
    }

    operator const std::bitset<8>() const
    {
        return factionMask;
    }

    bool operator==(const FactionComponent& other)
    {
        return factionMask == other.factionMask;
    }

    std::bitset<8> factionMask;
};

}  // namespace roen::ecs::components

#endif  // ROEN_ECS_COMPONENTS_FACTION_COMPONENT_HPP