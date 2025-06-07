#ifndef ROEN_RAYLIBAUDIOPLAYER_HPP
#define ROEN_RAYLIBAUDIOPLAYER_HPP

#include <core/AssetManager.hpp>
#include <interfaces/IAudioPlayer.hpp>

namespace roen
{

class RaylibAudioPlayer final : public interfaces::IAudioPlayer
{
public:
    RaylibAudioPlayer(std::shared_ptr<SoundManager> soundAssetManager,
                      std::shared_ptr<MusicManager> musicAssetManager);
    void playSound(std::string_view asset) override;
    void stopSound(std::string_view asset) override;
    void playMusic(std::string_view asset) override;
    void stopMusic(std::string_view asset) override;
    void pauseMusic() override;
    void resumeMusic() override;
    void updateMusicStream() override;

private:
    std::shared_ptr<SoundManager> soundAssetManager_;
    std::shared_ptr<MusicManager> musicAssetManager_;
    bool musicPlaying_;
};

}  // namespace roen

#endif  // ROEN_RAYLIBAUDIOPLAYER_HPP
