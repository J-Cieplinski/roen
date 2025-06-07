#include <core/AudioPlayer.hpp>

namespace roen
{

void AudioPlayer::PlaySound(std::string_view asset)
{
    impl_->playSound(asset);
}

void AudioPlayer::StopSound(std::string_view asset)
{
    impl_->stopSound(asset);
}

void AudioPlayer::PlayMusic(std::string_view asset)
{
    impl_->playMusic(asset);
}

void AudioPlayer::StopMusic(std::string_view asset)
{
    impl_->stopMusic(asset);
}

void AudioPlayer::PauseMusic()
{
    impl_->pauseMusic();
}

void AudioPlayer::ResumeMusic()
{
    impl_->resumeMusic();
}

void AudioPlayer::UpdateMusicStream()
{
    impl_->updateMusicStream();
}

void AudioPlayer::Init(std::unique_ptr<interfaces::IAudioPlayer> impl)
{
    impl_ = std::move(impl);
}

}  // namespace roen