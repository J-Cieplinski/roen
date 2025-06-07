#include <interfaces/Renderer.hpp>

#include <core/SceneGraph.hpp>
#include <ecs/components/DirtyComponent.hpp>
#include <ecs/components/GraphicsComponent.hpp>
#include <ecs/components/TransformComponent.hpp>

namespace roen
{

Renderer::Renderer(RenderContext context, std::shared_ptr<TextureManager> textureManager)
    : context_{context}
    , textureManager_{std::move(textureManager)}
{
}

void Renderer::updateTransforms(entt::registry& registry)
{
    auto hierarchyView = registry.view<ecs::TransformComponent, Hierarchy>();
    for (auto entity : hierarchyView)
    {
        auto& hierarchy = registry.get<Hierarchy>(entity);
        if (hierarchy.parent == entt::null)
        {
            updateChildrenTransforms(registry, entity);
        }
    }

    // Update non Hierarchy entities as well
    const auto view
        = registry.view<ecs::TransformComponent, ecs::GraphicsComponent>(entt::exclude<Hierarchy>);
    for (auto entity : view)
    {
        auto& transform = view.get<ecs::TransformComponent>(entity);
        transform.updateWorldTransform();
    }
}

void Renderer::updateChildrenTransforms(entt::registry& registry, entt::entity entity)
{
    auto& transform = registry.get<ecs::TransformComponent>(entity);
    auto& hierarchy = registry.get<Hierarchy>(entity);

    transform.updateWorldTransform();
    if (hierarchy.parent != entt::null)
    {
        const auto& parentTransform = registry.get<ecs::TransformComponent>(hierarchy.parent);
        transform.applyParent(parentTransform.worldTransform);
    }

    for (const auto child : hierarchy.children)
    {
        updateChildrenTransforms(registry, child);
    }
}

void Renderer::checkForDirtyAndSort(entt::registry& registry) const
{
    if (!registry.view<ecs::DirtyComponent>().empty())
    {
        APP_INFO("Detected dirty, sorting entities");

        registry.sort<ecs::TransformComponent>(
            [](const ecs::TransformComponent& lhs, const ecs::TransformComponent& rhs)
            { return lhs.zLayer < rhs.zLayer; });

        registry.clear<ecs::DirtyComponent>();
    }
}
}  // namespace roen