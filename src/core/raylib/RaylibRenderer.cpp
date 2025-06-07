#include <core/raylib/RaylibRenderer.hpp>

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
    : Renderer{std::move(context), std::move(textureManager)}
    , renderTexture_{LoadRenderTexture(context_.renderWidth, context_.renderHeight)}
{
}

void RaylibRenderer::onRender(entt::registry& registry)
{
    updateTransforms(registry);
    render(registry);
}

void RaylibRenderer::render(entt::registry& registry) const
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

void RaylibRenderer::renderEntities(entt::registry& registry) const
{
    checkForDirtyAndSort(registry);

    auto group = registry.group<ecs::TransformComponent, ecs::GraphicsComponent>();

    for (auto entity : group)
    {
        auto& transform = group.get<ecs::TransformComponent>(entity);
        auto& graphics = group.get<ecs::GraphicsComponent>(entity);

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

void RaylibRenderer::onRenderGui(entt::registry& registry)
{
    renderGui(registry);
}

void RaylibRenderer::renderGui(entt::registry&)
{
}

}  // namespace roen