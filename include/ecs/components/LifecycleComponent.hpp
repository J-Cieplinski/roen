#ifndef ROEN_ECS_LIFECYCLE_COMPONENT_HPP
#define ROEN_ECS_LIFECYCLE_COMPONENT_HPP

#include <cstdint>

namespace roen::ecs
{

enum class EntityState : std::uint8_t
{
    Alive,
    Dying,  // Running onDeath, death animation, effects
    Dead    // Ready for destruction
};

struct LifecycleComponent
{
    EntityState state = EntityState::Alive;
    float death_timer = 0.0f;
    float death_duration = 0.0f;

    bool isAlive() const { return state == EntityState::Alive; }
    bool isDying() const { return state == EntityState::Dying; }
    bool isDead() const { return state == EntityState::Dead; }
};

}  // namespace roen::ecs

#endif  // ROEN_ECS_FACTION_COMPONENT_HPP