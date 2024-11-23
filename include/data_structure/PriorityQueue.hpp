#ifndef ROEN_DATA_STRUCTURE_PRIORITYQUEUE_HPP
#define ROEN_DATA_STRUCTURE_PRIORITYQUEUE_HPP

#include <queue>
#include <utility>

namespace roen::data_structure
{

template<typename T, typename Priority>
class PriorityQueue
{
public:
    using PQElement = std::pair<Priority, T>;
    inline bool empty() const;
    inline void insert(PQElement&& element);
    inline void insert(const T& element, Priority priority);
    inline T get();
private:
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements_;
};

} // roen::data_structure

/*
 * Template implementation
 */

namespace roen::data_structure
{

template<typename T, typename Priority>
T PriorityQueue<T, Priority>::get()
{
    auto topElement = elements_.top().second;
    elements_.pop();
    return topElement;
}

template<typename T, typename Priority>
void PriorityQueue<T, Priority>::insert(PriorityQueue::PQElement&& element)
{
    elements_.emplace(element);
}

template<typename T, typename Priority>
void PriorityQueue<T, Priority>::insert(const T& element, Priority priority)
{
    elements_.emplace({priority, element});
}

template<typename T, typename Priority>
bool PriorityQueue<T, Priority>::empty() const
{
    return elements_.empty();
}

} // roen::data_structure

#endif //ROEN_DATA_STRUCTURE_PRIORITYQUEUE_HPP
