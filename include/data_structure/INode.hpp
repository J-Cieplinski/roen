#ifndef ROEN_DATA_STRUCTURE_INODE_HPP
#define ROEN_DATA_STRUCTURE_INODE_HPP

#include <data_structure/Vector2.hpp>

#include <cstdint>
#include <utility>

namespace roen::data_structure
{

class INode
{
public:
    virtual bool contains(const Vector2f& entity) const = 0;
    virtual std::uint32_t cost() const = 0;
};

} // roen::data_structure

#endif //ROEN_DATA_STRUCTURE_INODE_HPP