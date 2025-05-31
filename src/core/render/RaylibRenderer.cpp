#include <core/render/RaylibRenderer.hpp>

#include <core/SceneGraph.hpp>
#include <ecs/components/DirtyComponent.hpp>
#include <ecs/components/GraphicsComponent.hpp>
#include <ecs/components/TransformComponent.hpp>
#include <math/Types.hpp>
#include <math/decorators/RaylibDecorators.hpp>

#include <entt/entt.hpp>

#include <raylib.h>

namespace roen
{

RaylibRenderer::RaylibRenderer(RenderContext context,
                               std::shared_ptr<TextureManager> textureManager)
    : context_{std::move(context)}
    , renderTexture_{LoadRenderTexture(context_.renderWidth, context_.renderHeight)}
    , textureManager_{std::move(textureManager)}
{
}

void RaylibRenderer::onRender(entt::registry& registry)
{
    updateTransforms(registry);
    render(registry);
}

void RaylibRenderer::onRenderGui(entt::registry& registry)
{
    renderGui(registry);
}

void RaylibRenderer::updateTransforms(entt::registry& registry)
{
    auto hierarchyView = registry.view<ecs::components::TransformComponent, Hierarchy>();
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
        = registry.view<ecs::components::TransformComponent, ecs::components::GraphicsComponent>(
            entt::exclude<Hierarchy>);
    for (auto entity : view)
    {
        auto& transform = view.get<ecs::components::TransformComponent>(entity);
        transform.updateWorldTransform();
    }
}

void RaylibRenderer::updateChildrenTransforms(entt::registry& registry, entt::entity entity)
{
    auto& transform = registry.get<ecs::components::TransformComponent>(entity);
    auto& hierarchy = registry.get<Hierarchy>(entity);

    transform.updateWorldTransform();
    if (hierarchy.parent != entt::null)
    {
        const auto& parentTransform
            = registry.get<ecs::components::TransformComponent>(hierarchy.parent);
        transform.applyParent(parentTransform.worldTransform);
    }

    for (const auto child : hierarchy.children)
    {
        updateChildrenTransforms(registry, child);
    }
}

void RaylibRenderer::render(entt::registry& registry)
{
    float scale = std::min(static_cast<float>(GetScreenWidth()) / context_.renderWidth,
                           static_cast<float>(GetScreenHeight()) / context_.renderHeight);
    BeginTextureMode(renderTexture_);
    ClearBackground(YELLOW);

    renderEntities(registry);
    DrawFPS(0, 0);

    EndTextureMode();

    BeginDrawing();
    ClearBackground(WHITE);

    DrawTexturePro(renderTexture_.texture,
                   Rectangle{0.f, 0.f, static_cast<float>(renderTexture_.texture.width),
                             static_cast<float>(-(renderTexture_.texture.height))},
                   Rectangle{(GetScreenWidth() - (context_.renderWidth * scale)) * 0.5f,
                             (GetScreenHeight() - (context_.renderHeight * scale)) * 0.5f,
                             context_.renderWidth * scale, context_.renderHeight * scale},
                   Vector2{0, 0}, 0.f, WHITE);

    EndDrawing();
}

void RaylibRenderer::renderEntities(entt::registry& registry)
{
    checkForDirtyAndSort(registry);

    auto group
        = registry.group<ecs::components::TransformComponent, ecs::components::GraphicsComponent>();

    for (auto entity : group)
    {
        auto& transform = group.get<ecs::components::TransformComponent>(entity);
        auto& graphics = group.get<ecs::components::GraphicsComponent>(entity);

        const auto& worldTransform = transform.worldTransform;

        Rectangle dest{
            .x = worldTransform.getPosition().x,
            .y = worldTransform.getPosition().y,
            .width = graphics.srcRectangle.width * worldTransform.getScale().x,
            .height = graphics.srcRectangle.height * worldTransform.getScale().y,
        };

        Vector2 origin{
            .x = graphics.srcRectangle.width / 2.f,
            .y = graphics.srcRectangle.height / 2.f,
        };

        DrawTexturePro(textureManager_->getAsset(graphics.guid),
                       RaylibRectangle(graphics.srcRectangle), dest, origin,
                       worldTransform.getRotation() * math::TO_DEGREES, WHITE);
    }
}

void RaylibRenderer::renderGui(entt::registry& registry)
{
}

void RaylibRenderer::checkForDirtyAndSort(entt::registry& registry) const
{
    namespace ecs = ecs::components;
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