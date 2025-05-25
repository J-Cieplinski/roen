#ifndef ROEN_ASSETMANAGER_HPP
#define ROEN_ASSETMANAGER_HPP

#include <interfaces/IAsset.hpp>
#include <log/Logger.hpp>

#include <filesystem>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace roen
{

template <typename AssetType>
concept DerivedFromIAsset = std::is_base_of_v<interfaces::IAsset, AssetType> and requires {
    { AssetType::LoadFallbackAsset() } -> std::same_as<AssetType>;
};

class IAssetManager
{
public:
    virtual ~IAssetManager() = default;
    virtual void loadAsset(std::string_view id, const std::filesystem::path& path) = 0;
    virtual void freeAssets() = 0;
};

template <DerivedFromIAsset AssetType>
class AssetManager : public IAssetManager
{
public:
    AssetManager();
    ~AssetManager() override;

    void loadAsset(std::string_view id, const std::filesystem::path& path) override;
    void freeAssets() override;
    const AssetType& getAsset(std::string_view id) const;

private:
    std::unordered_map<std::uint64_t, AssetType> assets_;
};

}  // namespace roen

/*
 * Template definition
 */

#include <Utils.hpp>

namespace roen
{

template <DerivedFromIAsset AssetType>
AssetManager<AssetType>::AssetManager()
{
    auto hashedString = hashString(interfaces::DefaultAssetName);
    auto asset = AssetType::LoadFallbackAsset();

    assets_[hashedString] = asset;
}

template <DerivedFromIAsset AssetType>
AssetManager<AssetType>::~AssetManager()
{
    AssetManager::freeAssets();
}

template <DerivedFromIAsset AssetType>
void AssetManager<AssetType>::freeAssets()
{
    for (auto& [guid, asset] : assets_)
    {
        asset.freeAsset();
    }
    assets_.clear();
}

template <DerivedFromIAsset AssetType>
void AssetManager<AssetType>::loadAsset(std::string_view id, const std::filesystem::path& path)
{
    auto hashedString = hashString(id);
    if (assets_.contains(hashedString))
    {
        return;
    }

    AssetType asset;

    if (!asset.loadAsset(path))
    {
        std::stringstream ss;
        ss << "Failed to open asset with path: " << path << " and id: " << id;
        SDK_CRITICAL(ss.str());
        return;
    }

    assets_[hashedString] = asset;
}

template <DerivedFromIAsset AssetType>
const AssetType& AssetManager<AssetType>::getAsset(std::string_view id) const
{
    const auto hashedID = hashString(id);
    try
    {
        return assets_.at(hashedID);
    }
    catch (std::out_of_range&)
    {
        SDK_WARN("{0} id: \"{1}\" does not exist",
                 getDemangledName(std::type_index(typeid(AssetType)).name()), id);
        return assets_.at(interfaces::DefaultAssetID);
    }
}

}  // namespace roen

#include <core/Assets.hpp>

using TextureManager = roen::AssetManager<roen::TextureAsset>;
using FontManager = roen::AssetManager<roen::FontAsset>;
using SoundManager = roen::AssetManager<roen::SoundAsset>;
using MusicManager = roen::AssetManager<roen::MusicAsset>;

#endif  // ROEN_ASSETMANAGER_HPP