#ifndef SPIELDA_BASEMAPLOADER_HPP
#define SPIELDA_BASEMAPLOADER_HPP

#include <data_structure/Graph.hpp>
#include <data_structure/MapNode.hpp>
#include <data_structure/Vector2.hpp>
#include <manager/AssetManager.hpp>

#include <entt/entity/registry.hpp>

#include <json/single_include/nlohmann/json_fwd.hpp>
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

    void loadMap(const std::filesystem::path& path);
    [[nodiscard]] const data_structure::Vector2f& getMapSize() const;
    const data_structure::Graph<data_structure::MapNode>& getGraph() const;
protected:
    struct MapTile
    {
        std::int32_t cost {-1};
        data_structure::Vector2f position {0, 0};
    };

    static float getTileRotation(tson::Tile* tile);
    virtual void addComponents(data_structure::Vector2f tilePosition, data_structure::Vector2f tileSize, float rotation, tson::Rect drawingRect, int layerOrder,
                               const std::string& layerClass, const std::string& assetId) = 0;
    virtual manager::IAssetManager& getTextureManager() = 0;

    data_structure::Vector2f mapSize_;
    entt::registry& entityManager_;
    inline static std::regex reg{"(\\.\\.)"};
    data_structure::Graph<data_structure::MapNode> pathfindingGraph_;
private:
    void createPathfindingGraph(const std::vector<MapTile>& tiles, data_structure::Vector2f tileSize);
    nlohmann::json loadLevel(const std::filesystem::path& path);
};

} // roen::loader


#endif //SPIELDA_BASEMAPLOADER_HPP