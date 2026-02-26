#ifndef ROEN_APPLICATION_HPP
#define ROEN_APPLICATION_HPP

#include <core/AssetManager.hpp>
#include <manager/GameSceneManager.hpp>

#include <cstdint>
#include <memory>
#include <string>

namespace roen::interfaces
{
class IPlatform;
}

namespace roen
{

enum class PlatformType
{
    Raylib,
    SDL,
    SFML
};

class Renderer;

class Application
{
public:
    Application(std::uint32_t renderWidth, std::uint32_t renderHeight,
                std::unique_ptr<interfaces::IPlatform> platform);
    virtual ~Application();

    template <typename AssetManager>
    std::shared_ptr<AssetManager> getAssetManager() const;
    static std::set<PlatformType> getAvailablePlatforms();

    virtual void init() = 0;
    virtual void onRender(interfaces::Scene& scene);
    virtual void onGuiRender(interfaces::Scene& scene);

    void run();

protected:
    std::shared_ptr<TextureManager> textureManager_;
    std::shared_ptr<SoundManager> soundManager_;
    std::shared_ptr<FontManager> fontManager_;
    std::shared_ptr<MusicManager> musicManager_;
    manager::GameSceneManager gameSceneManager_;
    std::unique_ptr<interfaces::IPlatform> platform_;
    std::unique_ptr<Renderer> renderer_;
    bool isRunning_;
};

std::unique_ptr<Application> createApplication();

}  // namespace roen

#endif  // ROEN_APPLICATION_HPP
