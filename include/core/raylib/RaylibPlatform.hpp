#ifndef ROEN_RAYLIBPLATFORM_HPP
#define ROEN_RAYLIBPLATFORM_HPP

#include <interfaces/IPlatform.hpp>

namespace roen
{

class RaylibPlatform final : public interfaces::IPlatform
{
public:
    RaylibPlatform(std::uint32_t w, std::uint32_t h, std::string_view title)
        : width_{w}
        , height_{h}
        , title_{title}
    {
    }

    void init() override;
    void shutdown() override;
    bool shouldClose() override;
    std::unique_ptr<Renderer> createRenderer(
        RenderContext ctx, std::shared_ptr<TextureManager> textureManager) override;
    std::unique_ptr<interfaces::IAudioPlayer> createAudioPlayer(
        std::shared_ptr<SoundManager> soundManager,
        std::shared_ptr<MusicManager> musicManager) override;

private:
    std::uint32_t width_;
    std::uint32_t height_;
    std::string title_;
};

}  // namespace roen

#endif  // ROEN_RAYLIBPLATFORM_HPP
