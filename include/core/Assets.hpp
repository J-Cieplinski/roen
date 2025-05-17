#ifndef ROEN_CORE_ASSETS_HPP
#define ROEN_CORE_ASSETS_HPP

#include <interfaces/IAsset.hpp>

#include <raylib.h>

namespace roen
{

class TextureAsset final : public interfaces::Asset<Texture2D>
{
public:
    bool loadAsset(const std::filesystem::path& path) override;
    static TextureAsset LoadFallbackAsset();
    void freeAsset() override;
};

class FontAsset final : public interfaces::Asset<Font>
{
public:
    bool loadAsset(const std::filesystem::path& path) override;
    static FontAsset LoadFallbackAsset();
    void freeAsset() override;
};

class SoundAsset final : public interfaces::Asset<Sound>
{
public:
    bool loadAsset(const std::filesystem::path& path) override;
    static SoundAsset LoadFallbackAsset();
    void freeAsset() override;
};

class MusicAsset final : public interfaces::Asset<Music>
{
public:
    bool loadAsset(const std::filesystem::path& path) override;
    static MusicAsset LoadFallbackAsset();
    void freeAsset() override;
};

}  // namespace roen

#endif  // ROEN_CORE_ASSETS_HPP
