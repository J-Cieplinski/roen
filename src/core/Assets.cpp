#include <core/Assets.hpp>

#include <core/DefaultAsset.hpp>
#include <log/Logger.hpp>

namespace roen
{

bool TextureAsset::loadAsset(const std::filesystem::path& path)
{
    SDK_INFO("Loading texture image: {0}", path.string());

    asset_ = LoadTexture(path.c_str());
    assetPath_ = path.filename().string();
    return IsTextureValid(asset_);
}

void TextureAsset::freeAsset()
{
    SDK_INFO("Freeing texture: {0}", assetPath_.c_str());

    if (IsTextureValid(asset_))
    {
        UnloadTexture(asset_);
    }
}

TextureAsset TextureAsset::LoadFallbackAsset()
{
    TextureAsset asset;
    asset.assetPath_ = interfaces::DefaultAssetName;

    Image image{.data = asset::placeholders::DEFAULT_TEXTURE_DATA,
                .width = asset::placeholders::DEFAULT_TEXTURE_WIDTH,
                .height = asset::placeholders::DEFAULT_TEXTURE_HEIGHT,
                .mipmaps = 1,
                .format = asset::placeholders::DEFAULT_TEXTURE_FORMAT};

    asset.asset_ = LoadTextureFromImage(image);
    if (not IsTextureValid(asset))
    {
        throw std::runtime_error("Failed to load default texture");
    }

    return asset;
}

bool FontAsset::loadAsset(const std::filesystem::path& path)
{
    SDK_INFO("Loading font: {0}", path.string());

    asset_ = LoadFont(path.c_str());
    assetPath_ = path.filename().string();

    return asset_.texture.id != 0 and IsFontValid(asset_);
}

FontAsset FontAsset::LoadFallbackAsset()
{
    FontAsset asset;
    asset.assetPath_ = interfaces::DefaultAssetName;
    asset.asset_ = GetFontDefault();
    if (not IsFontValid(asset))
    {
        throw std::runtime_error("Failed to load default font");
    }

    return asset;
}

void FontAsset::freeAsset()
{
    SDK_INFO("Unloading font: {0}", assetPath_.c_str());

    UnloadFont(asset_);
}

bool SoundAsset::loadAsset(const std::filesystem::path& path)
{
    SDK_INFO("Loading sound: {0}", path.string());

    asset_ = LoadSound(path.c_str());
    assetPath_ = path.filename().string();

    return IsSoundValid(asset_);
}

void SoundAsset::freeAsset()
{
    SDK_INFO("Unloading sound: {0}", assetPath_.c_str());

    if (IsSoundValid(asset_))
    {
        UnloadSound(asset_);
    }
}

SoundAsset SoundAsset::LoadFallbackAsset()
{
    SoundAsset asset;
    asset.assetPath_ = interfaces::DefaultAssetName;
    const auto wave = LoadWaveFromMemory(".wav", asset::placeholders::DEFAULT_SOUND,
                                         sizeof(asset::placeholders::DEFAULT_SOUND));
    asset.asset_ = LoadSoundFromWave(wave);
    if (not IsSoundValid(asset))
    {
        throw std::runtime_error("Failed to load default sound");
    }

    return asset;
}

bool MusicAsset::loadAsset(const std::filesystem::path& path)
{
    SDK_INFO("Loading sound: {0}", path.string());

    asset_ = LoadMusicStream(path.c_str());
    assetPath_ = path.filename().string();

    return IsMusicValid(asset_);
}

MusicAsset MusicAsset::LoadFallbackAsset()
{
    MusicAsset asset;
    asset.assetPath_ = interfaces::DefaultAssetName;
    asset.asset_ = LoadMusicStreamFromMemory(".wav", asset::placeholders::DEFAULT_SOUND,
                                             sizeof(asset::placeholders::DEFAULT_SOUND));
    if (not IsMusicValid(asset))
    {
        throw std::runtime_error("Failed to load default music");
    }

    return asset;
}

void MusicAsset::freeAsset()
{
    SDK_INFO("Unloading music: {0}", assetPath_.c_str());

    if (IsMusicValid(asset_))
    {
        if (IsMusicStreamPlaying(asset_))
        {
            StopMusicStream(asset_);
        }
        UnloadMusicStream(asset_);
    }
}

}  // namespace roen