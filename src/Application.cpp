#include <roenpch.hpp>

#include <Application.hpp>

#include <core/AudioPlayer.hpp>
#include <core/render/RaylibRenderer.hpp>

namespace roen
{

Application::Application(std::uint32_t windowWith, std::uint32_t windowHeight,
                         std::uint32_t renderWidth, std::uint32_t renderHeight,
                         std::string_view windowTitle)
    : isRunning_{true}
{
#ifdef IS_DEBUG
    log::Logger::Init();
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWith, windowHeight, windowTitle.data());
    InitAudioDevice();
    textureManager_ = std::make_shared<TextureManager>();
    soundManager_ = std::make_shared<SoundManager>();
    musicManager_ = std::make_shared<MusicManager>();
    fontManager_ = std::make_shared<FontManager>();

    RenderContext context{
        .renderWidth = renderWidth,
        .renderHeight = renderHeight,
    };

    renderer_ = std::make_unique<RaylibRenderer>(context, textureManager_);

    AudioPlayer::Init(soundManager_, musicManager_);
}

Application::~Application()
{
    gameSceneManager_.shutdown();
    CloseAudioDevice();
    CloseWindow();
}

void Application::onRender()
{
    renderer_->onRender(gameSceneManager_.getCurrentScene().getEntityManager());
}

void Application::onGuiRender()
{
    renderer_->onRenderGui(gameSceneManager_.getCurrentScene().getEntityManager());
}

void Application::run()
{
    SDK_INFO("Started main loop");

    while (isRunning_)
    {
        try
        {
            isRunning_ = !WindowShouldClose();
            gameSceneManager_.update();
            auto& currentScene = gameSceneManager_.getCurrentScene();
            AudioPlayer::UpdateMusicStream();
            currentScene.handleInput();
            currentScene.update();

            onRender();
            onGuiRender();
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