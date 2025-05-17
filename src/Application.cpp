#include <roenpch.hpp>

#include <Application.hpp>

namespace roen
{

Application::Application(std::uint32_t windowWith, std::uint32_t windowHeight,
                         const std::string& windowTitle)
    : isRunning_{true}
{
#ifdef IS_DEBUG
    log::Logger::Init();
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWith, windowHeight, windowTitle.c_str());
    InitAudioDevice();
    textureManager_ = std::make_shared<TextureManager>();
    soundManager_ = std::make_shared<SoundManager>();
    musicManager_ = std::make_shared<MusicManager>();
    fontManager_ = std::make_shared<FontManager>();
}

Application::~Application()
{
    gameSceneManager_.shutdown();
    CloseAudioDevice();
    CloseWindow();
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
            currentScene.handleInput();
            currentScene.update();
            currentScene.render();
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