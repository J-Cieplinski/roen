#include <ecs/Entity.hpp>

#include <entt/entity/registry.hpp>

namespace roen::ecs
{

Entity::Entity(entt::entity entity, entt::registry& registry)
    : entity_{entity}
    , registry_{registry}
{
}

Entity::operator bool() const
{
    return entity_ != entt::null;
}

Entity::operator entt::entity() const
{
    return entity_;
}

bool Entity::operator==(const Entity& other) const
{
    return entity_ == other.entity_;
}

}  // namespace roen::ecs