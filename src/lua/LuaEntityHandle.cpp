#include <lua/LuaEntityHandle.hpp>

#include <ecs/Entity.hpp>
#include <ecs/components/TransformComponent.hpp>

#include <log/Logger.hpp>

namespace roen::lua
{

LuaEntityHandle::LuaEntityHandle(ecs::Entity& entity)
    : entity_{entity}
{
}

void LuaEntityHandle::setTransform(std::uint32_t x, std::uint32_t y)
{
    auto transform = entity_.getComponentMaybe<ecs::components::TransformComponent>();
    if (not transform)
    {
        APP_WARN(transform.error());
        APP_WARN("Entity {} does not have TransformComponent", entity_);
        return;
    }

    auto& transformComponent = transform.value().get();

    transformComponent.transform.x = x;
    transformComponent.transform.y = y;
}

}  // namespace roen::lua
