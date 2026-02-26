#include <ecs/components/LifecycleComponent.hpp>
#include <ecs/systems/LifecycleSystem.hpp>
#include <log/Logger.hpp>
#include <lua/LuaScript.hpp>

namespace roen::ecs
{

std::vector<entt::entity> LifecycleSystem::update(float dt) const
{
    std::vector<entt::entity> entities_to_destroy;

    auto& registry = scene_->getEntityManager().getRegistry();
    auto view = registry.view<LifecycleComponent, lua::LuaScript>();

    for (const auto [entity_handle, lifecycle, script] : view.each())
    {
        if (lifecycle.state == EntityState::Dying and lifecycle.triggered_on_death)
        {
            // Tick death timer
            lifecycle.death_timer += dt;

            // Check if death complete
            if (lifecycle.death_timer >= lifecycle.death_duration)
            {
                lifecycle.state = EntityState::Dead;
                entities_to_destroy.push_back(entity_handle);
                SDK_TRACE("Entity {0} death complete, marking for destruction", entity_handle);
            }
        }
    }

    return entities_to_destroy;
}

}  // namespace roen::ecs
