#ifndef SPIELDA_BASEMAPLOADER_HPP
#define SPIELDA_BASEMAPLOADER_HPP

#include <data_structure/Graph.hpp>
#include <data_structure/MapNode.hpp>
#include <manager/AssetManager.hpp>

#include <entt/entity/registry.hpp>

#include <json/single_include/nlohmann/json_fwd.hpp>
#include <raymath.h>
#include <tileson/tileson.hpp>

#include <filesystem>
#include <regex>

namespace roen::loader
{

struct LayerTypes
{
    inline static std::string BACKGROUND {"BACKGROUND"};
    inline static std::string TRIGGERS {"TRIGGERS"};
    inline static std::string COLLIDABLE {"COLLIDABLE"};
};

struct LayerProperties
{
    inline static std::string RENDER_ORDER {"renderOrder"};
};

struct TileProperties
{
    inline static std::string MOVEMENT_COST {"movementCost"};
};

class BaseMapLoader
{
public:
    explicit BaseMapLoader(entt::registry& entityManager);
    virtual ~BaseMapLoader() = default;

    void loadMap(const std::filesystem::path& path);
    [[nodiscard]] const Vector2& getMapSize() const;
    [[nodiscard]] const Vector2& getTileSize() const;
    const data_structure::Graph<data_structure::MapNode>& getGraph() const;
protected:
    struct MapTile
    {
        std::int32_t cost {-1};
        Vector2 position {0, 0};
    };

    static float getTileRotation(tson::Tile* tile);
    virtual void addComponents(Vector2 tilePosition, Vector2 tileSize, float rotation, tson::Rect drawingRect, int layerOrder,
                               const std::string& layerClass, const std::string& assetId) = 0;
    virtual manager::IAssetManager& getTextureManager() = 0;

    Vector2 mapSize_;
    Vector2 tileSize_;
    entt::registry& entityManager_;
    inline static std::regex reg{"(\\.\\.)"};
    data_structure::Graph<data_structure::MapNode> pathfindingGraph_;
private:
    void createPathfindingGraph(const std::vector<MapTile>& tiles, Vector2 tileSize);
    nlohmann::json loadLevel(const std::filesystem::path& path);
};

} // roen::loader


#endif //SPIELDA_BASEMAPLOADER_HPP