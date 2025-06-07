#ifndef ROEN_INTERFACES_IAUDIOPLAYER_HPP
#define ROEN_INTERFACES_IAUDIOPLAYER_HPP

namespace roen::interfaces
{

class IAudioPlayer
{
public:
    virtual ~IAudioPlayer() = default;
    virtual void playSound(std::string_view asset) = 0;
    virtual void stopSound(std::string_view asset) = 0;
    virtual void playMusic(std::string_view asset) = 0;
    virtual void stopMusic(std::string_view asset) = 0;
    virtual void pauseMusic() = 0;
    virtual void resumeMusic() = 0;
    virtual void updateMusicStream() = 0;
};

}  // namespace roen::interfaces

#endif  // ROEN_INTERFACES_IAUDIOPLAYER_HPP
