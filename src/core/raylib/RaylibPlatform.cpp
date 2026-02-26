#include <core/raylib/RaylibPlatform.hpp>

#include <core/raylib/RaylibAudioPlayer.hpp>
#include <core/raylib/RaylibRenderer.hpp>

namespace roen
{

void RaylibPlatform::init()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width_, height_, title_.c_str());
    InitAudioDevice();
}

void RaylibPlatform::shutdown()
{
    CloseAudioDevice();
    CloseWindow();
}

bool RaylibPlatform::shouldClose()
{
    return WindowShouldClose();
}

std::unique_ptr<Renderer> RaylibPlatform::createRenderer(
    RenderContext ctx, std::shared_ptr<TextureManager> textureManager)
{
    return std::make_unique<RaylibRenderer>(ctx, std::move(textureManager));
}

std::unique_ptr<interfaces::IAudioPlayer> RaylibPlatform::createAudioPlayer(
    std::shared_ptr<SoundManager> soundManager, std::shared_ptr<MusicManager> musicManager)
{
    return std::make_unique<RaylibAudioPlayer>(std::move(soundManager), std::move(musicManager));
}

}  // namespace roen