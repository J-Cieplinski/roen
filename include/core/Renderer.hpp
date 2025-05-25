#ifndef ROEN_RENDERER_HPP
#define ROEN_RENDERER_HPP

#include <raylib.h>
#include <entt/fwd.hpp>

namespace roen
{

struct RenderContext
{
    std::uint32_t renderWidth;
    std::uint32_t renderHeight;
};

class Renderer
{
public:
    Renderer(RenderContext context);

    void onRender(entt::registry& registry);
    void onRenderGui(entt::registry& registry);

private:
    void render(entt::registry& registry);
    void renderGui(entt::registry& registry);

    RenderContext context_;
    RenderTexture renderTexture_;
};

}  // namespace roen

#endif  // ROEN_RENDERER_HPP
