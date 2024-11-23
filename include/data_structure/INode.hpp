#ifndef ROEN_DATA_STRUCTURE_INODE_HPP
#define ROEN_DATA_STRUCTURE_INODE_HPP

#include <raymath.h>

#include <cstdint>

namespace roen::data_structure
{

class INode
{
public:
    virtual ~INode() = default;
    virtual bool contains(const Vector2& entity) const = 0;
    virtual std::uint32_t cost() const = 0;
};

} // roen::data_structure

#endif //ROEN_DATA_STRUCTURE_INODE_HPP