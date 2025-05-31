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

class RaylibRenderer
{
public:
    RaylibRenderer(RenderContext context, std::shared_ptr<TextureManager> textureManager);

    void onRender(entt::registry& registry);
    void onRenderGui(entt::registry& registry);

private:
    void render(entt::registry& registry);
    void renderEntities(entt::registry& registry);
    void renderGui(entt::registry& registry);
    void updateTransforms(entt::registry& registry);
    void updateChildrenTransforms(entt::registry& registry, entt::entity entity);
    void checkForDirtyAndSort(entt::registry& registry) const;

    RenderContext context_;
    RenderTexture renderTexture_;
    std::shared_ptr<TextureManager> textureManager_;
};

}  // namespace roen

#endif  // ROEN_RENDERER_HPP
