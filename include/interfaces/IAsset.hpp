#ifndef ROEN_INTERFACES_IASSET_HPP
#define ROEN_INTERFACES_IASSET_HPP

#include <Utils.hpp>

#include <filesystem>

namespace roen::interfaces
{

constexpr std::string_view DefaultAssetName{"DefaultAsset"};
constexpr std::uint64_t DefaultAssetID{hashString(DefaultAssetName)};

class IAsset
{
public:
    virtual ~IAsset() = default;

    virtual bool loadAsset(const std::filesystem::path& path) = 0;
    virtual void freeAsset() = 0;
};

template <typename AssetType>
class Asset : public IAsset
{
public:
    virtual ~Asset() = default;
    [[nodiscard]]
    virtual AssetType get() const
    {
        return asset_;
    }
    operator AssetType() const
    {
        return get();
    }
    std::string_view getPath() const
    {
        return assetPath_;
    }

protected:
    AssetType asset_;
    std::string assetPath_;
};

}  // namespace roen::interfaces

#endif  // ROEN_INTERFACES_IASSET_HPP
