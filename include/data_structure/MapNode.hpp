#ifndef ROEN_DATA_STRUCTURE_MAPNODE_HPP
#define ROEN_DATA_STRUCTURE_MAPNODE_HPP

#include <data_structure/INode.hpp>

#include <cstdint>

#include <spdlog/fmt/bundled/ostream.h>

namespace roen::data_structure
{

class MapNode : public INode
{
public:
    MapNode();
    MapNode(const Vector2& position, const Vector2& size, std::uint32_t cost);
    MapNode(const MapNode& other);
    friend bool operator==(const MapNode& lhs, const MapNode& rhs);
    friend bool operator<(const MapNode& lhs, const MapNode& rhs);
    Vector2 operator-(const MapNode& other) const;

    bool contains(const Vector2& entity) const override;
    std::uint32_t cost() const override;
    const Vector2& getPosition() const;
    const Vector2& getSize() const;

    friend std::ostream& operator<<(std::ostream& os, const MapNode& node);
private:
    friend std::hash<MapNode>;
    Vector2 position_;
    Vector2 size_;
    std::uint32_t movementCost_;
};

} // roen::data_structure

// Hashing
template <>
struct std::hash<roen::data_structure::MapNode>
{
    size_t operator()(const roen::data_structure::MapNode& node) const
    {
        auto xTemp = node.position_.x;
        auto yTemp = node.position_.y;
        return std::hash<size_t>()(std::hash<float>{}(xTemp) ^ (std::hash<float>{}(yTemp)));
    }
};

// For spdlog print support
template <> struct fmt::formatter<roen::data_structure::MapNode> : ostream_formatter {};

#endif //ROEN_DATA_STRUCTURE_MAPNODE_HPP