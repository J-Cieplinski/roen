#ifndef ROEN_INTERFACES_IASSET_HPP
#define ROEN_INTERFACES_IASSET_HPP

#include <filesystem>

namespace roen::interfaces
{

class IAsset
{
public:
    virtual bool loadAsset(const std::filesystem::path& path) = 0;
    virtual void freeAsset() = 0;
};

template <typename AssetType>
class Asset : public IAsset
{
public:
    virtual ~Asset() = default;
    virtual AssetType get() const = 0;
    operator AssetType() const { return get(); };
};

} // roen::interfaces

#endif //ROEN_INTERFACES_IASSET_HPP
