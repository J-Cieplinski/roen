#ifndef ROEN_APPLICATION_HPP
#define ROEN_APPLICATION_HPP

#include <core/AssetManager.hpp>
#include <manager/GameSceneManager.hpp>

#include <cstdint>
#include <memory>
#include <string>

namespace roen
{

class Application
{
public:
    Application(std::uint32_t windowWith, std::uint32_t windowHeight,
                const std::string& windowTitle = "Roen");
    virtual ~Application();

    template <typename AssetManager>
    std::shared_ptr<AssetManager> getAssetManager() const;

    virtual void init() = 0;

    void run();

protected:
    std::shared_ptr<TextureManager> textureManager_;
    std::shared_ptr<SoundManager> soundManager_;
    std::shared_ptr<FontManager> fontManager_;
    std::shared_ptr<MusicManager> musicManager_;
    manager::GameSceneManager gameSceneManager_;
    bool isRunning_;
};

std::unique_ptr<Application> createApplication();

}  // namespace roen

#endif  // ROEN_APPLICATION_HPP
