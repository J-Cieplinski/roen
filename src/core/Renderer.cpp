#include <core/Renderer.hpp>

#include <entt/entt.hpp>

#include <raylib.h>

namespace roen
{

Renderer::Renderer(RenderContext context)
    : context_(std::move(context))
    , renderTexture_(LoadRenderTexture(context_.renderWidth, context_.renderHeight))
{
}

void Renderer::onRender(entt::registry& registry)
{
    render(registry);
}

void Renderer::onRenderGui(entt::registry& registry)
{
    renderGui(registry);
}

void Renderer::render(entt::registry& registry)
{
    float scale = std::min(static_cast<float>(GetScreenWidth()) / context_.renderWidth,
                           static_cast<float>(GetScreenHeight()) / context_.renderHeight);

    BeginTextureMode(renderTexture_);
    ClearBackground(YELLOW);

    DrawCircle(context_.renderWidth / 2, context_.renderHeight / 2, 10.f, RED);
    DrawRectangle(context_.renderWidth / 2, context_.renderHeight / 2, 10, 10, BLUE);
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

void Renderer::renderGui(entt::registry& registry)
{
}

}  // namespace roen