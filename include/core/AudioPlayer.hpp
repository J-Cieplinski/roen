#ifndef ROEN_CORE_SOUND_PLAYER_HPP
#define ROEN_CORE_SOUND_PLAYER_HPP

#include <core/AssetManager.hpp>

#include <string_view>

namespace roen
{

class AudioPlayer
{
public:
    static void Init(std::shared_ptr<SoundManager> soundAssetManager,
                     std::shared_ptr<MusicManager> musicAssetManager);

    static void PlaySound(std::string_view asset);
    static void StopSound(std::string_view asset);

    static void PlayMusic(std::string_view asset);
    static void StopMusic(std::string_view asset);
    static void PauseMusic();
    static void ResumeMusic();
    static void UpdateMusicStream();

private:
    inline static std::shared_ptr<SoundManager> soundAssetManager_;
    inline static std::shared_ptr<MusicManager> musicAssetManager_;
    inline static bool musicPlaying_{false};
};

}  // namespace roen

#endif  // ROEN_CORE_SOUND_PLAYER_HPP
