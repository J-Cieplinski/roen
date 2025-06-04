#include <core/AudioPlayer.hpp>

#include <raylib.h>

namespace roen
{

namespace
{
std::string_view activeMusic;
}

void AudioPlayer::PlaySound(std::string_view asset)
{
    SDK_INFO("Playing sound \"{0}\"", asset);
    ::PlaySound(soundAssetManager_->getAsset(asset));
}

void AudioPlayer::StopSound(std::string_view asset)
{
    SDK_INFO("Stopping sound \"{0}\"", asset);
    ::StopSound(soundAssetManager_->getAsset(asset));
}

void AudioPlayer::PlayMusic(std::string_view asset)
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

void AudioPlayer::StopMusic(std::string_view asset)
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

void AudioPlayer::PauseMusic()
{
    if (activeMusic.empty() or not musicPlaying_)
    {
        return;
    }

    SDK_INFO("Pausing music \"{0}\"", activeMusic);
    PauseMusicStream(musicAssetManager_->getAsset(activeMusic));
    musicPlaying_ = false;
}

void AudioPlayer::ResumeMusic()
{
    if (!activeMusic.empty() and musicPlaying_)
    {
        return;
    }
    SDK_INFO("Resuming music \"{0}\"", activeMusic);
    ResumeMusicStream(musicAssetManager_->getAsset(activeMusic));
    musicPlaying_ = true;
}

void AudioPlayer::UpdateMusicStream()
{
    if (!activeMusic.empty())
    {
        ::UpdateMusicStream(musicAssetManager_->getAsset(activeMusic));
    }
}

void AudioPlayer::Init(std::shared_ptr<SoundManager> soundAssetManager,
                       std::shared_ptr<MusicManager> musicAssetManager)
{
    soundAssetManager_ = std::move(soundAssetManager);
    musicAssetManager_ = std::move(musicAssetManager);
}

}  // namespace roen