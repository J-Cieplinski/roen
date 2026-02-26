#ifndef ROEN_INTERFACES_IPLATFORM_HPP
#define ROEN_INTERFACES_IPLATFORM_HPP

#include <memory>

namespace roen::interfaces
{

class IPlatform
{
public:
    virtual ~IPlatform() = default;

    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual bool shouldClose() = 0;

    virtual std::unique_ptr<Renderer> createRenderer(RenderContext, std::shared_ptr<TextureManager>)
        = 0;
    virtual std::unique_ptr<IAudioPlayer> createAudioPlayer(std::shared_ptr<SoundManager>,
                                                            std::shared_ptr<MusicManager>)
        = 0;
};

}  // namespace roen::interfaces

#endif  // ROEN_INTERFACES_IPLATFORM_HPP
