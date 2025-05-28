#include <ecs/Entity.hpp>

#include <core/SceneGraph.hpp>

namespace roen::ecs
{

Entity::Entity(entt::entity entity, entt::registry& registry)
    : entity_{entity}
    , registry_{registry}
{
}

void Entity::addChild(const Entity child) const
{
    SDK_INFO("Adding child {0} to entity {1}", child, *this);
    updateHierarchy(*this, child);
}

void Entity::setParent(const Entity parent) const
{
    SDK_INFO("Setting parent {0} to entity {1}", parent, *this);
    updateHierarchy(parent, *this);
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

void Entity::updateHierarchy(const Entity parent, const Entity child) const
{
    auto& parentHierarchy = registry_.get_or_emplace<Hierarchy>(parent);
    parentHierarchy.children.insert(child);

    auto& childHierarchy = registry_.get_or_emplace<Hierarchy>(child);
    if (auto maybeChildParentHierarchy = registry_.try_get<Hierarchy>(childHierarchy.parent))
    {
        maybeChildParentHierarchy->children.erase(child);
    }
    childHierarchy.parent = parent;
}

}  // namespace roen::ecs