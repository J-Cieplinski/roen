#include <../../../include/core/AssetManager.hpp>

#include <interfaces/IAsset.hpp>

#include <gtest/gtest.h>

namespace roen
{

namespace
{
const std::string FALSE_PATH{"FALSE"};
const std::string DUMMY_PATH{"DUMMY_PATH"};
const std::string DUMMY_ID{"DUMMY_ID"};
const std::string DUMMY_PATH_2{"DUMMY_PATH_2"};
const std::string DUMMY_ID_2{"DUMMY_ID_2"};

constexpr std::uint32_t VAL_FALSE{0};
constexpr std::uint32_t VAL_1{1};
constexpr std::uint32_t VAL_2{2};
constexpr float DEFAULT_FLOAT_VAL{123.123f};
constexpr std::uint32_t DEFAULT_INT_VAL{123};

const std::map<std::string, int> pathToVal
    = {{FALSE_PATH, VAL_FALSE}, {DUMMY_PATH, VAL_1}, {DUMMY_PATH_2, VAL_2}};

}  // namespace

namespace interfaces
{

template <int val>
class StubAsset : public Asset<int>
{
public:
    bool loadAsset(const std::filesystem::path& path) override
    {
        asset_ = pathToVal.at(path);

        return asset_;
    }
    [[nodiscard]] int get() const override { return asset_; }
    void freeAsset() override {}
    static const StubAsset& LoadFallbackAsset()
    {
        static auto asset = []
        {
            StubAsset asset;
            asset.asset_ = DEFAULT_INT_VAL;
            return asset;
        }();

        return asset;
    }

private:
    int asset_{val};
};

class FloatStubAsset : public Asset<float>
{
public:
    bool loadAsset(const std::filesystem::path&) override { return asset_; }
    [[nodiscard]] float get() const override { return asset_; }
    void freeAsset() override {}
    static const FloatStubAsset& LoadFallbackAsset()
    {
        static auto asset = []
        {
            FloatStubAsset asset;
            asset.asset_ = DEFAULT_FLOAT_VAL;
            return asset;
        }();

        return asset;
    }

private:
    float asset_{5.0f};
};

}  // namespace interfaces

namespace manager
{

class AssetManagerTests : public ::testing::TestWithParam<std::pair<bool, bool>>
{
};

TEST_F(AssetManagerTests, loadAsset_ShouldLoadAsset)
{
    auto sut = AssetManager<interfaces::StubAsset<VAL_1>>();
    EXPECT_NO_THROW(sut.loadAsset(DUMMY_ID, DUMMY_PATH));
    EXPECT_EQ(sut.getAsset(DUMMY_ID), VAL_1);
}

TEST_F(AssetManagerTests, free_ShouldUnloadAssets)
{
    auto sut = AssetManager<interfaces::StubAsset<VAL_1>>();
    EXPECT_NO_THROW(sut.loadAsset(DUMMY_ID, DUMMY_PATH));
    EXPECT_EQ(sut.getAsset(DUMMY_ID), VAL_1);
    EXPECT_NO_THROW(sut.freeAssets());
    EXPECT_EQ(sut.getAsset(DUMMY_ID), DEFAULT_INT_VAL);
}

TEST_F(AssetManagerTests, getAsset_ShouldReturnDefaultAssetOnAccessingNotPresentAsset)
{
    auto sut = AssetManager<interfaces::StubAsset<VAL_1>>();
    EXPECT_EQ(sut.getAsset(DUMMY_ID), DEFAULT_INT_VAL);
}

TEST_F(AssetManagerTests, getAsset_ShouldReturnCorrectAsset)
{
    auto sut = AssetManager<interfaces::StubAsset<VAL_1>>();
    EXPECT_NO_THROW(sut.loadAsset(DUMMY_ID, DUMMY_PATH));
    EXPECT_NO_THROW(sut.loadAsset(DUMMY_ID_2, DUMMY_PATH_2));
    EXPECT_EQ(sut.getAsset(DUMMY_ID), VAL_1);
    EXPECT_EQ(sut.getAsset(DUMMY_ID_2), VAL_2);
}

TEST_F(AssetManagerTests, AssetContainerIsTheDifferentForDifferentTypesOfManager)
{
    auto sut = AssetManager<interfaces::StubAsset<VAL_1>>();
    EXPECT_NO_THROW(sut.loadAsset(DUMMY_ID, DUMMY_PATH));

    auto sutOne = AssetManager<interfaces::FloatStubAsset>();

    EXPECT_EQ(sut.getAsset(DUMMY_ID), VAL_1);
    EXPECT_EQ(sutOne.getAsset(DUMMY_ID), DEFAULT_FLOAT_VAL);
}

}  // namespace manager

}  // namespace roen