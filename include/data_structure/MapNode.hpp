#ifndef ROEN_DATA_STRUCTURE_MAPNODE_HPP
#define ROEN_DATA_STRUCTURE_MAPNODE_HPP

#include <data_structure/INode.hpp>

#include <cstdint>
#include <unordered_map>

namespace roen::data_structure
{

class MapNode : public INode
{
public:
    MapNode();
    MapNode(const Vector2f& position, const Vector2f& size, std::uint32_t cost);
    MapNode(const MapNode& other);
    friend bool operator==(const MapNode& lhs, const MapNode& rhs);
    friend bool operator<(const MapNode& lhs, const MapNode& rhs);
    Vector2f operator-(const MapNode& other) const;

    bool contains(const Vector2f& entity) const override;
    std::uint32_t cost() const override;
    const Vector2f& getPosition() const;
    const Vector2f& getSize() const;
private:
    friend std::hash<MapNode>;
    Vector2f position_;
    Vector2f size_;
    std::uint32_t movementCost_;
};

} // roen::data_structure

namespace std
{
    template <>
    struct hash<roen::data_structure::MapNode>
    {
        size_t operator()(const roen::data_structure::MapNode& node) const
        {
            auto xTemp = node.position_.x;
            auto yTemp = node.position_.y;
            return std::hash<size_t>()(std::hash<float>{}(xTemp) ^ (std::hash<float>{}(yTemp)));
        }
    };
} // std

#endif //ROEN_DATA_STRUCTURE_MAPNODE_HPP