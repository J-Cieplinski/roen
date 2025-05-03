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
    registry_.on_construct<components::GraphicsComponent>()
        .connect<[&](entt::registry& reg, entt::entity e)
                 { reg.emplace<components::DirtyComponent>(e); }>();
}

Entity EntityManager::createEntity()
{
    Entity entity{registry_.create(), *this};
    entity.addComponent<components::TransformComponent>();
    APP_INFO("Created entity {}", entity);

    return entity;
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