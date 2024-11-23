#include <loader/BaseMapLoader.hpp>

#include <data_structure/Vector2.hpp>

#include <log/Logger.hpp>

#include <json/single_include/nlohmann/json.hpp>

#include <ranges>
#include <regex>

namespace roen::loader
{

using json = nlohmann::json;

using Vector2f = data_structure::Vector2f;

template <typename T>
data_structure::Vector2f toVector2f(const tson::Vector2<T> v)
{
    return {static_cast<float>(v.x), static_cast<float>(v.y)};
}

BaseMapLoader::BaseMapLoader(entt::registry& entityManager)
    : mapSize_{0.f, 0.f}
    , entityManager_{entityManager}
{
}

void BaseMapLoader::loadMap(const std::filesystem::path& path)
{
    auto level = loadLevel(path);
    const std::string assetId = level["mapName"];
    const std::filesystem::path mapPath{level["mapFile"]};

    tson::Tileson tileson;
    auto map = tileson.parse(mapPath);
    mapSize_ = toVector2f(map->getSize());
    std::vector<MapTile> mappedTiles{};

    auto tileSize = toVector2f(map->getTileSize());

    auto& textureManager = getTextureManager();

    if(map->getStatus() == tson::ParseStatus::OK)
    {
        for(auto& layer : map->getLayers())
        {
            auto layerOrderProp = layer.getProp(LayerProperties::RENDER_ORDER);

            if(!layerOrderProp)
            {
                SDK_CRITICAL("Lack of layerOrderProp in layer: {0} of map: {1}", layer.getName(), mapPath.string());
                return;
            }

            auto layerOrder = std::any_cast<int>(layerOrderProp->getValue());

            auto layerClass = layer.getClassType();

            for (auto& [pos, tile]: layer.getTileData()) {

                auto tilePosition = toVector2f(tile->getPosition(pos));

                if(layerClass == LayerTypes::BACKGROUND)
                {
                    if(auto tileMoveCost = tile->getProp(TileProperties::MOVEMENT_COST))
                    {
                        mappedTiles.emplace_back(std::any_cast<int>(tileMoveCost->getValue()), tilePosition);
                    }
                }

                auto drawingRect = tile->getDrawingRect();

                auto imagePath = std::regex_replace(tile->getTileset()->getFullImagePath().string(), reg, "assets");
                textureManager.loadAsset(assetId, imagePath);

                auto rotation = getTileRotation(tile);

                addComponents(tilePosition, tileSize, rotation, drawingRect, layerOrder, layerClass, assetId);
            }
        }

        createPathfindingGraph(mappedTiles, tileSize);
    }
}

float BaseMapLoader::getTileRotation(tson::Tile *tile)
{
    auto flippedDiagonally = tile->hasFlipFlags(tson::TileFlipFlags::Diagonally);
    auto flippedHorizontally = tile->hasFlipFlags(tson::TileFlipFlags::Horizontally);
    auto flippedVertically = tile->hasFlipFlags(tson::TileFlipFlags::Vertically);

    if(flippedDiagonally)
    {
        flippedHorizontally = flippedVertically;
        flippedVertically = !tile->hasFlipFlags(tson::TileFlipFlags::Horizontally);
        if(flippedVertically && flippedHorizontally)
        {
            return 270.f;
        }
        return 90.f;
    }

    if(flippedVertically && flippedHorizontally)
    {
        return 180.f;
    }

    return 0.f;
}

const data_structure::Vector2f& BaseMapLoader::getMapSize() const
{
    return mapSize_;
}

void BaseMapLoader::createPathfindingGraph(const std::vector<MapTile>& tiles, Vector2f tileSize)
{
    //                                                          NW                              W                                  SW
    std::vector<Vector2f> DIRECTIONS_WITH_DIAGONAL = {{-tileSize.x, -tileSize.y}, {-tileSize.x, 0}, {-tileSize.x, tileSize.y},
    //                                                      N                       S
                                            {0, -tileSize.y}, {0, tileSize.y},
    //                                                      NE                                  E                                   SE
                                            {tileSize.x, -tileSize.y}, {tileSize.x, 0}, {tileSize.x, tileSize.y}};

    const std::vector<Vector2f> DIRECTIONS = {
    /*W*/   {-tileSize.x, 0},
    /*N*/   {0, -tileSize.y},
    /*S*/   {0, tileSize.y},
    /*E*/   {tileSize.x, 0} };

    auto findTile = [&tiles](Vector2f position) {
        return std::ranges::find_if(tiles, [&position](const MapTile& checkedTile) -> bool {
            return checkedTile.position == position;
        });
    };

    auto tileExist = [&](auto checkedTile) {
        return checkedTile != tiles.end();
    };

    for(const auto& tile : tiles)
    {
        std::vector<data_structure::MapNode> edges {};
        for(const auto& direction : DIRECTIONS)
        {
            Vector2f neighbourPos {tile.position.x + direction.x, tile.position.y + direction.y};
            auto mapTile = findTile(neighbourPos);
            if(tileExist(mapTile))
            {
                edges.emplace_back(neighbourPos, tileSize, static_cast<std::uint32_t>(mapTile->cost));
            }
        }

        pathfindingGraph_.addNode({tile.position, tileSize, static_cast<std::uint32_t>(tile.cost)}, edges);
    }
}

nlohmann::json BaseMapLoader::loadLevel(const std::filesystem::path& path)
{
    SDK_INFO("Loading level: {0}", path.string());

    std::ifstream file{path};
    if(!file.is_open())
    {
        SDK_CRITICAL("Failed to open level file: {0}", path.string());
        return {};
    }

    return json::parse(file);
}

const data_structure::Graph<data_structure::MapNode>& BaseMapLoader::getGraph() const
{
    return pathfindingGraph_;
}

} // roen::loader
