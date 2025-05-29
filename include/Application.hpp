#ifndef ROEN_APPLICATION_HPP
#define ROEN_APPLICATION_HPP

#include <core/AssetManager.hpp>
#include <manager/GameSceneManager.hpp>

#include <cstdint>
#include <memory>
#include <string>

namespace roen
{

class RaylibRenderer;

class Application
{
public:
    Application(std::uint32_t windowWith, std::uint32_t windowHeight, std::uint32_t renderWidth,
                std::uint32_t renderHeight, std::string_view windowTitle = "Roen");
    virtual ~Application();

    template <typename AssetManager>
    std::shared_ptr<AssetManager> getAssetManager() const;

    virtual void init() = 0;
    virtual void onRender();
    virtual void onGuiRender();

    void run();

protected:
    std::shared_ptr<TextureManager> textureManager_;
    std::shared_ptr<SoundManager> soundManager_;
    std::shared_ptr<FontManager> fontManager_;
    std::shared_ptr<MusicManager> musicManager_;
    manager::GameSceneManager gameSceneManager_;
    std::unique_ptr<RaylibRenderer> renderer_;
    bool isRunning_;
};

std::unique_ptr<Application> createApplication();

}  // namespace roen

#endif  // ROEN_APPLICATION_HPP
