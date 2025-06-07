#include <ecs/EntityManager.hpp>

#include <ecs/Entity.hpp>
#include <ecs/components/DirtyComponent.hpp>
#include <ecs/components/GraphicsComponent.hpp>
#include <ecs/components/TransformComponent.hpp>

#include <log/Logger.hpp>

namespace roen::ecs
{

EntityManager::EntityManager()
{
    registry_.on_construct<TransformComponent>()
        .connect<[&](entt::registry& reg, entt::entity e) { reg.emplace<DirtyComponent>(e); }>();
}

Entity EntityManager::createEntity()
{
    Entity entity{registry_.create(), &registry_};
    SDK_INFO("Created entity {}", entity);

    return entity;
}

Entity EntityManager::getEntity()
{
    Entity entity{registry_.create(), &registry_};
    SDK_INFO("Returning entity {}", entity);

    return entity;
}

void EntityManager::clear()
{
    registry_.clear();
}

EntityManager::operator entt::registry&()
{
    return registry_;
}

const entt::registry& EntityManager::getRegistry() const
{
    return registry_;
}

entt::registry& EntityManager::getRegistry()
{
    return registry_;
}

}  // namespace roen::ecs