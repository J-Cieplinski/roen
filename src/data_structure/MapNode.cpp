#include <data_structure/MapNode.hpp>

namespace roen::data_structure
{

MapNode::MapNode() = default;

MapNode::MapNode(const MapNode& other) = default;

MapNode::MapNode(const Vector2& position, const Vector2& size, std::uint32_t cost)
    : position_{position}
    , size_{size}
    , movementCost_{cost}
{
}

bool MapNode::contains(const Vector2& entity) const
{
    return position_.x <= entity.x && (position_.x + size_.x) >= entity.x && position_.y <= entity.y
           && (position_.y + size_.y) >= entity.y;
}

std::uint32_t MapNode::cost() const
{
    return movementCost_;
}

bool operator==(const MapNode& lhs, const MapNode& rhs)
{
    return lhs.position_ == rhs.position_ && lhs.size_ == rhs.size_
           && lhs.movementCost_ == rhs.movementCost_;
}

Vector2 MapNode::operator-(const MapNode& other) const
{
    return position_ - other.position_;
}

bool operator<(const MapNode& lhs, const MapNode& rhs)
{
    return std::tie(lhs.position_.x, lhs.position_.y) < std::tie(rhs.position_.x, rhs.position_.y);
}

std::ostream& operator<<(std::ostream& os, const MapNode& node)
{
    os << "\n\t{";
    os << "\n\t\tPosition (" << node.position_.x << ", " << node.position_.y << ")";
    os << "\n\t\tSize (" << node.size_.x << ", " << node.size_.y << ")";
    os << "\n\t\tMovement Cost (" << node.movementCost_ << ")";
    os << "\n\t}";
    return os;
}

const Vector2& MapNode::getPosition() const
{
    return position_;
}

const Vector2& MapNode::getSize() const
{
    return size_;
}

}  // namespace roen::data_structure