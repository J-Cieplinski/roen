#ifndef ROEN_LUA_READ_ONLY_ECS_HPP
#define ROEN_LUA_READ_ONLY_ECS_HPP

#include <ecs/Entity.hpp>
#include <ecs/EntityManager.hpp>
#include <ecs/components/FactionComponent.hpp>
#include <ecs/components/GraphicsComponent.hpp>
#include <ecs/components/TransformComponent.hpp>
#include <interfaces/Scene.hpp>
#include <lua/CommandSystem.hpp>

#include <optional>
#include <shared_mutex>
#include <sol2/sol.hpp>
#include <unordered_map>

namespace roen::lua
{

struct TransformSnapshot
{
    math::Vector2 position{0, 0};
    math::Vector2 scale{1, 1};
    float rotation = 0;
};

struct GraphicsSnapshot
{
    std::string texture_id;
    math::Rectangle src_rect;
    std::uint8_t z_layer = 0;
};

struct FactionSnapshot
{
    std::uint8_t mask = 0;
};

class EntityQueryCache
{
public:
    explicit EntityQueryCache(interfaces::Scene* scene)
        : scene_(scene)
    {
    }

    std::optional<TransformSnapshot> getTransform(entt::entity id)
    {
        {
            std::shared_lock lock(mutex_);
            auto it = transforms_.find(id);
            if (it != transforms_.end())
            {
                return it->second;
            }
        }

        auto entity = scene_->getEntityManager().getEntity(id);
        if (!entity.hasComponent<ecs::TransformComponent>())
        {
            return std::nullopt;
        }

        auto& t = entity.getComponent<ecs::TransformComponent>();
        TransformSnapshot snap{t.position, t.scale, t.rotation};

        std::unique_lock lock(mutex_);
        transforms_[id] = snap;
        return snap;
    }

    std::optional<GraphicsSnapshot> getGraphics(entt::entity id)
    {
        {
            std::shared_lock lock(mutex_);
            auto it = graphics_.find(id);
            if (it != graphics_.end())
            {
                return it->second;
            }
        }

        auto entity = scene_->getEntityManager().getEntity(id);
        if (!entity.hasComponent<ecs::GraphicsComponent>())
        {
            return std::nullopt;
        }

        auto& g = entity.getComponent<ecs::GraphicsComponent>();
        GraphicsSnapshot snap{g.guid, g.srcRectangle};

        std::unique_lock lock(mutex_);
        graphics_[id] = snap;
        return snap;
    }

    std::optional<FactionSnapshot> getFaction(entt::entity id)
    {
        {
            std::shared_lock lock(mutex_);
            auto it = factions_.find(id);
            if (it != factions_.end())
            {
                return it->second;
            }
        }

        auto entity = scene_->getEntityManager().getEntity(id);
        if (!entity.hasComponent<ecs::FactionComponent>())
        {
            return std::nullopt;
        }

        auto& f = entity.getComponent<ecs::FactionComponent>();
        FactionSnapshot snap{static_cast<std::uint8_t>(f.factionMask.to_ulong())};

        std::unique_lock lock(mutex_);
        factions_[id] = snap;
        return snap;
    }

    bool exists(entt::entity id) const { return scene_->getEntityManager().getEntity(id).valid(); }

    void clear()
    {
        std::unique_lock lock(mutex_);
        transforms_.clear();
        graphics_.clear();
        factions_.clear();
    }

private:
    interfaces::Scene* scene_;
    mutable std::shared_mutex mutex_;
    std::unordered_map<entt::entity, TransformSnapshot> transforms_;
    std::unordered_map<entt::entity, GraphicsSnapshot> graphics_;
    std::unordered_map<entt::entity, FactionSnapshot> factions_;
};

class EntityView
{
public:
    EntityView(entt::entity id, EntityQueryCache* cache)
        : id_(id)
        , cache_(cache)
    {
    }

    entt::entity getId() const { return id_; }

    bool isValid() const { return cache_->exists(id_); }

    bool hasTransform() const { return cache_->getTransform(id_).has_value(); }

    bool hasGraphics() const { return cache_->getGraphics(id_).has_value(); }

    bool hasFaction() const { return cache_->getFaction(id_).has_value(); }

    sol::object getPos(sol::this_state s) const
    {
        auto t = cache_->getTransform(id_);
        if (!t) return sol::nil;
        return sol::make_object(s, t->position);
    }

    sol::object getRotation(sol::this_state s) const
    {
        auto t = cache_->getTransform(id_);
        if (!t) return sol::nil;
        return sol::make_object(s, t->rotation);
    }

    sol::object getScale(sol::this_state s) const
    {
        auto t = cache_->getTransform(id_);
        if (!t) return sol::nil;
        return sol::make_object(s, t->scale);
    }

    sol::object getTexture(sol::this_state s) const
    {
        auto g = cache_->getGraphics(id_);
        if (!g) return sol::nil;
        return sol::make_object(s, g->texture_id);
    }

    sol::object getSrcRect(sol::this_state s) const
    {
        auto g = cache_->getGraphics(id_);
        if (!g) return sol::nil;
        return sol::make_object(s, g->src_rect);
    }

    sol::object getFactionMask(sol::this_state s) const
    {
        auto f = cache_->getFaction(id_);
        if (!f) return sol::nil;
        return sol::make_object(s, f->mask);
    }

private:
    entt::entity id_;
    EntityQueryCache* cache_;
};

class SceneQuery
{
public:
    explicit SceneQuery(EntityQueryCache* cache)
        : cache_(cache)
    {
    }

    EntityView getEntity(entt::entity id) const { return EntityView(id, cache_); }

    bool exists(entt::entity id) const { return cache_->exists(id); }

private:
    EntityQueryCache* cache_;
};

}  // namespace roen::lua

#endif
