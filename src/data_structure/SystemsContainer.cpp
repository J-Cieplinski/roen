#include <data_structure/SystemsContainer.hpp>

namespace roen::data_structure
{

SystemsContainer::SystemsIterator SystemsContainer::begin()
{
    return systems_.begin();
}

SystemsContainer::SystemsConstIterator SystemsContainer::begin() const
{
    return systems_.begin();
}

SystemsContainer::SystemsConstIterator SystemsContainer::cbegin() const
{
    return systems_.cbegin();
}

SystemsContainer::SystemsIterator SystemsContainer::end()
{
    return systems_.end();
}

SystemsContainer::SystemsConstIterator SystemsContainer::end() const
{
    return systems_.end();
}

SystemsContainer::SystemsConstIterator SystemsContainer::cend() const
{
    return systems_.cend();
}
} // roen::data_structure
