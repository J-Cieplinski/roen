#ifndef ROEN_RENDERER_HPP
#define ROEN_RENDERER_HPP

#include <interfaces/Renderer.hpp>

#include <raylib.h>
#include <entt/fwd.hpp>

namespace roen
{

class RaylibRenderer final : public Renderer
{
public:
    RaylibRenderer(RenderContext context, std::shared_ptr<TextureManager> textureManager);

    void onRender(entt::registry& registry) override;
    void onRenderGui(entt::registry& registry) override;

private:
    void render(entt::registry& registry) const;
    void renderEntities(entt::registry& registry) const;
    void renderGui(entt::registry& registry);

    RenderTexture renderTexture_;
};

}  // namespace roen

#endif  // ROEN_RENDERER_HPP
