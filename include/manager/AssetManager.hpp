#ifndef SPIELDA_ASSETMANAGER_HPP
#define SPIELDA_ASSETMANAGER_HPP

#include <interfaces/IAsset.hpp>
#include <log/Logger.hpp>

#include <filesystem>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace roen::manager
{

template <typename AssetType>
concept DerivedFromIAsset = std::is_base_of_v<interfaces::IAsset, AssetType>;

class IAssetManager
{
public:
    virtual ~IAssetManager() = default;
    virtual void loadAsset(const std::string& id, const std::filesystem::path& path) = 0;
    virtual void freeAssets() = 0;
};

template <DerivedFromIAsset AssetType>
class AssetManager : public IAssetManager
{
public:
    ~AssetManager() override;

    void loadAsset(const std::string& id, const std::filesystem::path& path) override;
    void freeAssets() override;
    AssetType& getAsset(std::uint64_t id) const;

private:
    inline static std::unordered_map<std::uint64_t, AssetType> assets_;
};

}  // namespace roen::manager

/*
 * Template definition
 */

#include <Utils.hpp>

namespace roen::manager
{

template <DerivedFromIAsset AssetType>
AssetManager<AssetType>::~AssetManager()
{
    AssetManager<AssetType>::freeAssets();
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
void AssetManager<AssetType>::loadAsset(const std::string& id, const std::filesystem::path& path)
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
        throw std::runtime_error(ss.str());
    }

    assets_[hashedString] = asset;
}

template <DerivedFromIAsset AssetType>
AssetType& AssetManager<AssetType>::getAsset(std::uint64_t id) const
{
    try
    {
        return assets_.at(id);
    }
    catch (std::out_of_range& e)
    {
        SDK_CRITICAL("{0} id: {1} does not exist", std::type_index(typeid(AssetType)).name(), id);
        throw e;
    }
}

}  // namespace roen::manager

#endif  // SPIELDA_ASSETMANAGER_HPP