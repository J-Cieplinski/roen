#ifndef ROEN_ECS_ENTITY_HPP
#define ROEN_ECS_ENTITY_HPP

#include <Utils.hpp>
#include <log/formatters/entity.hpp>

#include <entt/entity/fwd.hpp>

#include <expected>
#include <functional>

namespace roen::ecs
{

enum class Error
{
    COMPONENT_MISSING
};

class Entity
{
public:
    Entity();
    Entity(entt::entity entity, entt::registry* registry);

    template <typename Component, typename... Args>
    Component& addComponent(Args... args);

    template <typename Component>
    bool hasComponent() const;

    template <typename Component>
    Component& getComponent();

    template <typename Component>
    void removeComponent() const;

    void addChild(Entity child) const;
    void setParent(Entity parent) const;

    template <typename Component>
    [[nodiscard("Error should be handled")]]
    std::expected<std::reference_wrapper<Component>, Error> getComponentMaybe();

    operator bool() const;
    operator entt::entity() const;

    bool operator==(const Entity&) const;

private:
    void updateHierarchy(const Entity parent, const Entity child) const;

    entt::entity entity_;
    entt::registry* registry_;
};

}  // namespace roen::ecs

/*
 * Template definition
 */

namespace roen::ecs
{

template <typename Component, typename... Args>
Component& Entity::addComponent(Args... args)
{
    SDK_INFO("Adding component: {} to entity: {}",
             getDemangledName(std::type_index(typeid(Component)).name()), entity_);
    return registry_->emplace<Component>(entity_, std::forward<Args>(args)...);
}

template <typename Component>
bool Entity::hasComponent() const
{
    auto component = registry_->try_get<Component>(entity_);
    return component != nullptr;
}

template <typename Component>
Component& Entity::getComponent()
{
    return registry_->get<Component>(entity_);
}

template <typename Component>
void Entity::removeComponent() const
{
    registry_->remove<Component>(entity_);
}

template <typename Component>
std::expected<std::reference_wrapper<Component>, Error> Entity::getComponentMaybe()
{
    auto component = registry_->try_get<Component>(entity_);

    if (component != nullptr)
    {
        return *component;
    }

    return std::unexpected{Error::COMPONENT_MISSING};
}

}  // namespace roen::ecs

#include <fmt/format.h>

namespace fmt
{

template <>
struct formatter<roen::ecs::Entity> : formatter<string_view>
{
public:
    inline auto format(roen::ecs::Entity entt, format_context& ctx) const;
};

inline auto formatter<roen::ecs::Entity>::format(roen::ecs::Entity entity,
                                                 format_context& ctx) const
{
    formatter<entt::entity> entityFormat;
    return entityFormat.format(static_cast<entt::entity>(entity), ctx);
}

template <>
struct formatter<roen::ecs::Error> : formatter<string_view>
{
public:
    inline auto format(roen::ecs::Error error, format_context& ctx) const;
};

#define ERROR_CASE(errorType, str)      \
    {                                   \
        using Error = roen::ecs::Error; \
        case Error::errorType:          \
            str = #errorType;           \
            break;                      \
    }

inline auto formatter<roen::ecs::Error>::format(roen::ecs::Error error, format_context& ctx) const
{
    string_view enttStr{"UNKNOWN ERROR"};
    switch (error)
    {
        ERROR_CASE(COMPONENT_MISSING, enttStr)
    }

    return formatter<string_view>::format(enttStr, ctx);
}

}  // namespace fmt

#endif  // ROEN_ECS_ENTITY_HPP