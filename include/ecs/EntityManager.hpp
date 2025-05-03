#ifndef ROEN_ECS_ENTITY_MANAGER_HPP
#define ROEN_ECS_ENTITY_MANAGER_HPP

#include <entt/entity/registry.hpp>

namespace roen::ecs
{

class Entity;

class EntityManager
{
public:
    EntityManager();
    Entity createEntity();

    operator entt::registry&();

    const entt::registry& getRegistry() const;
    entt::registry& getRegistry();

private:
    entt::registry registry_;
};

}  // namespace roen::ecs

#endif  // ROEN_ECS_ENTITY_MANAGER_HPP