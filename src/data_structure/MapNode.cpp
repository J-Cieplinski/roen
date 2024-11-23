#include <data_structure/MapNode.hpp>

namespace roen::data_structure
{

MapNode::MapNode() = default;

MapNode::MapNode(const MapNode& other) = default;

MapNode::MapNode(const Vector2f& position, const Vector2f& size, std::uint32_t cost)
    : position_{position}
    , size_{size}
    , movementCost_{cost}
{
}

bool MapNode::contains(const Vector2f& entity) const
{
    return position_.x <= entity.x
        && (position_.x + size_.x) >= entity.x
        && position_.y <= entity.y
        && (position_.y + size_.y) >= entity.y;
}

std::uint32_t MapNode::cost() const
{
    return movementCost_;
}

bool operator==(const MapNode& lhs, const MapNode& rhs)
{
    return lhs.position_ == rhs.position_
        && lhs.size_ == rhs.size_
        && lhs.movementCost_ == rhs.movementCost_;
}

Vector2f MapNode::operator-(const MapNode& other) const
{
    return position_ - other.position_;
}

bool operator<(const MapNode& lhs, const MapNode& rhs)
{
    return lhs.position_ < rhs.position_;
}

const Vector2f& MapNode::getPosition() const
{
    return position_;
}

const Vector2f& MapNode::getSize() const
{
    return size_;
}

} // roen::data_structure