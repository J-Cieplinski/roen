#ifndef ROEN_DATA_STRUCTURE_SYSTEMSCONTAINER_HPP
#define ROEN_DATA_STRUCTURE_SYSTEMSCONTAINER_HPP

#include <interfaces/ISystem.hpp>
#include <log/Logger.hpp>

#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace roen::data_structure
{

class SystemsContainer
{

using SystemsMap = std::unordered_map<std::type_index, std::unique_ptr<interfaces::ISystem>>;
using SystemsIterator = SystemsMap::iterator;
using SystemsConstIterator = SystemsMap::const_iterator;
public:
    template<std::derived_from<interfaces::ISystem> SystemType, typename ...Args>
    void add(Args&& ...args);

    template<std::derived_from<interfaces::ISystem> SystemType>
    void remove();

    template<std::derived_from<interfaces::ISystem> SystemType>
    [[nodiscard]] bool hasSystem() const;

    template<std::derived_from<interfaces::ISystem> SystemType>
    SystemType& get() const;

    [[nodiscard]] SystemsIterator begin();
    [[nodiscard]] SystemsConstIterator begin() const;
    [[nodiscard]] SystemsConstIterator cbegin() const;

    [[nodiscard]] SystemsIterator end();
    [[nodiscard]] SystemsConstIterator end() const;
    [[nodiscard]] SystemsConstIterator cend() const;
private:
    SystemsMap systems_;
};

} // roen::data_structure

/*
 * Template implementation
 */

#include <Utils.hpp>

namespace roen::data_structure
{

template<std::derived_from<interfaces::ISystem> SystemType, typename... Args>
void SystemsContainer::add(Args&&... args)
{
    auto newSystem = std::make_unique<SystemType>(std::forward<Args>(args)...);
    const auto key = std::type_index(typeid(SystemType));

#ifdef __GNUC__
    const std::string name {getDemangledName(key.name())};
#else
    const std::string name {key.name()};
#endif

    SDK_INFO("Adding system of type: {0}", name);

    systems_.insert({ key, std::move(newSystem) });
}

template<std::derived_from<interfaces::ISystem> SystemType>
void SystemsContainer::remove()
{
    const auto key = std::type_index(typeid(SystemType));
    if(systems_.erase(key))
    {
        SDK_INFO("Removing system of type: {0}", key.name());
    }
}

template<std::derived_from<interfaces::ISystem> SystemType>
bool SystemsContainer::hasSystem() const
{
    const auto key = std::type_index(typeid(SystemType));
    return systems_.contains(key);
}

template<std::derived_from<interfaces::ISystem> SystemType>
SystemType& SystemsContainer::get() const
{
    const auto key = std::type_index(typeid(SystemType));
    auto system = systems_.find(key);

    if(system == systems_.end())
    {
        std:: stringstream ss;
        ss << "No system of type: " << key.name() << " present";
        throw std::out_of_range(ss.str());
    }

    return static_cast<SystemType&>(*(system->second));
}

} // roen::data_structure

#endif //ROEN_DATA_STRUCTURE_SYSTEMSCONTAINER_HPP