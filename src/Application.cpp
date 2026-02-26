#include <roenpch.hpp>

#include <Application.hpp>

#include <core/AudioPlayer.hpp>

#include "core/raylib/RaylibPlatform.hpp"

namespace roen
{

Application::Application(std::uint32_t renderWidth, std::uint32_t renderHeight,
                         std::unique_ptr<interfaces::IPlatform> platform)
    : platform_{std::move(platform)}
    , isRunning_{true}
{
#ifdef IS_DEBUG
    log::Logger::Init();
#endif

    textureManager_ = std::make_shared<TextureManager>();
    soundManager_ = std::make_shared<SoundManager>();
    musicManager_ = std::make_shared<MusicManager>();
    fontManager_ = std::make_shared<FontManager>();

    const RenderContext context{
        .renderWidth = renderWidth,
        .renderHeight = renderHeight,
    };

    platform_->init();
    renderer_ = platform_->createRenderer(context, textureManager_);
    AudioPlayer::Init(platform_->createAudioPlayer(soundManager_, musicManager_));
}

Application::~Application()
{
    gameSceneManager_.shutdown();
    AudioPlayer::Shutdown();
    textureManager_.reset();
    soundManager_.reset();
    musicManager_.reset();
    fontManager_.reset();
    renderer_.reset();
    platform_->shutdown();
}

std::set<PlatformType> Application::getAvailablePlatforms()
{
    static std::set availablePlatforms{PlatformType::Raylib};

    return availablePlatforms;
}

void Application::onRender(interfaces::Scene& scene)
{
    renderer_->onRender(scene.getEntityManager());
}

void Application::onGuiRender(interfaces::Scene& scene)
{
    renderer_->onRenderGui(scene.getEntityManager());
}

void Application::run()
{
    SDK_INFO("Started main loop");

    while (isRunning_)
    {
        try
        {
            isRunning_ = !platform_->shouldClose();
            gameSceneManager_.update();
            auto& currentScene = gameSceneManager_.getCurrentScene();
            AudioPlayer::UpdateMusicStream();
            currentScene.handleInput();
            currentScene.update();

            onRender(currentScene);
            onGuiRender(currentScene);
        }
        catch (std::exception& e)
        {
            SDK_CRITICAL(e.what());
            return;
        }
    }
}

template <>
std::shared_ptr<MusicManager> Application::getAssetManager() const
{
    return musicManager_;
}

template <>
std::shared_ptr<TextureManager> Application::getAssetManager() const
{
    return textureManager_;
}

template <>
std::shared_ptr<SoundManager> Application::getAssetManager() const
{
    return soundManager_;
}

template <>
std::shared_ptr<FontManager> Application::getAssetManager() const
{
    return fontManager_;
}

}  // namespace roen