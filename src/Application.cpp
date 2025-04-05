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
}

Application::~Application()
{
    gameSceneManager_.shutdown();
    CloseWindow();
}

void Application::run()
{
    APP_INFO("Started main loop");

    while (isRunning_)
    {
        isRunning_ = !WindowShouldClose();
        gameSceneManager_.update();
        auto& currentScene = gameSceneManager_.getCurrentScene();
        currentScene.handleInput();
        currentScene.update();
        currentScene.render();
    }
}

}  // namespace roen