#ifndef ROEN_CORE_SOUND_PLAYER_HPP
#define ROEN_CORE_SOUND_PLAYER_HPP

#include <interfaces/IAudioPlayer.hpp>

#include <string_view>

namespace roen
{

class AudioPlayer
{
public:
    static void Init(std::unique_ptr<interfaces::IAudioPlayer> impl);

    static void PlaySound(std::string_view asset);
    static void StopSound(std::string_view asset);

    static void PlayMusic(std::string_view asset);
    static void StopMusic(std::string_view asset);
    static void PauseMusic();
    static void ResumeMusic();
    static void UpdateMusicStream();

private:
    inline static std::unique_ptr<interfaces::IAudioPlayer> impl_;
};

}  // namespace roen

#endif  // ROEN_CORE_SOUND_PLAYER_HPP
