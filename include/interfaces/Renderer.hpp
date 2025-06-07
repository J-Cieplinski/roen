#ifndef ROEN_INTERFACES_RENDERER_HPP
#define ROEN_INTERFACES_RENDERER_HPP

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
    Renderer(RenderContext context, std::shared_ptr<TextureManager> textureManager);
    virtual ~Renderer() = default;

    virtual void onRender(entt::registry& registry) = 0;
    virtual void onRenderGui(entt::registry& registry) = 0;

protected:
    void updateTransforms(entt::registry& registry);
    void updateChildrenTransforms(entt::registry& registry, entt::entity entity);
    void checkForDirtyAndSort(entt::registry& registry) const;

    RenderContext context_;
    std::shared_ptr<TextureManager> textureManager_;
};

}  // namespace roen

#endif  // ROEN_INTERFACES_RENDERER_HPP
