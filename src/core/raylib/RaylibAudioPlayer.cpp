#include <core/raylib/RaylibAudioPlayer.hpp>

#include <raylib.h>

namespace roen
{

namespace
{
std::string_view activeMusic;
}

RaylibAudioPlayer::RaylibAudioPlayer(std::shared_ptr<SoundManager> soundAssetManager,
                                     std::shared_ptr<MusicManager> musicAssetManager)
    : soundAssetManager_{std::move(soundAssetManager)}
    , musicAssetManager_{std::move(musicAssetManager)}
    , musicPlaying_{false}
{
}

void RaylibAudioPlayer::playSound(std::string_view asset)
{
    SDK_INFO("Playing sound \"{0}\"", asset);
    ::PlaySound(soundAssetManager_->getAsset(asset));
}

void RaylibAudioPlayer::stopSound(std::string_view asset)
{
    SDK_INFO("Stopping sound \"{0}\"", asset);
    ::StopSound(soundAssetManager_->getAsset(asset));
}

void RaylibAudioPlayer::playMusic(std::string_view asset)
{
    SDK_INFO("Playing music \"{0}\"", asset);
    auto music = musicAssetManager_->getAsset(asset);
    if (not IsMusicValid(music))
    {
        SDK_WARN("Music \"{0}\" is not valid", asset);
    }
    PlayMusicStream(music);
    activeMusic = asset;
    musicPlaying_ = true;
}

void RaylibAudioPlayer::stopMusic(std::string_view asset)
{
    if (activeMusic.empty())
    {
        return;
    }

    SDK_INFO("Stopping music \"{0}\"", asset);
    StopMusicStream(musicAssetManager_->getAsset(asset));
    musicPlaying_ = false;
    activeMusic = "";
}

void RaylibAudioPlayer::pauseMusic()
{
    if (activeMusic.empty() or not musicPlaying_)
    {
        return;
    }

    SDK_INFO("Pausing music \"{0}\"", activeMusic);
    PauseMusicStream(musicAssetManager_->getAsset(activeMusic));
    musicPlaying_ = false;
}

void RaylibAudioPlayer::resumeMusic()
{
    if (activeMusic.empty() or musicPlaying_)
    {
        return;
    }
    SDK_INFO("Resuming music \"{0}\"", activeMusic);
    ResumeMusicStream(musicAssetManager_->getAsset(activeMusic));
    musicPlaying_ = true;
}

void RaylibAudioPlayer::updateMusicStream()
{
    if (!activeMusic.empty())
    {
        ::UpdateMusicStream(musicAssetManager_->getAsset(activeMusic));
    }
}

}  // namespace roen
