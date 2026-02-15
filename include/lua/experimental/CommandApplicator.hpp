#ifndef ROEN_LUA_COMMAND_APPLICATOR_HPP
#define ROEN_LUA_COMMAND_APPLICATOR_HPP

#include <Application.hpp>
#include <core/AssetManager.hpp>
#include <core/AudioPlayer.hpp>
#include <interfaces/Scene.hpp>
#include <log/Logger.hpp>
#include <lua/experimental/CommandSystem.hpp>
#include <lua/experimental/LuaScript.hpp>

namespace roen::lua::experimental
{

class LuaScript;

inline entt::entity resolveTarget(const Target& t,
                                  const std::unordered_map<Handle, entt::entity>& handle_map)
{
    if (std::holds_alternative<entt::entity>(t))
    {
        return std::get<entt::entity>(t);  // existing entity
    }

    Handle const h = std::get<Handle>(t);
    if (auto const it = handle_map.find(h); it != handle_map.end())
    {
        return it->second;  // handle → entt::entity
    }
    return entt::null;  // unresolved
}

inline void applyCommands(const std::vector<Command>& cmds, interfaces::Scene* scene,
                          std::unordered_map<Handle, entt::entity>& handle_map)
{
    for (auto& cmd : cmds)
    {
        std::visit(
            [&](auto&& c)
            {
                using T = std::decay_t<decltype(c)>;

                if constexpr (std::is_same_v<T, LoadTextureCmd>)
                {
                    scene->getApplication()->getAssetManager<TextureManager>()->loadAsset(c.id,
                                                                                          c.path);
                }
                else if constexpr (std::is_same_v<T, LoadSoundCmd>)
                {
                    scene->getApplication()->getAssetManager<SoundManager>()->loadAsset(c.id,
                                                                                        c.path);
                }
                else if constexpr (std::is_same_v<T, LoadMusicCmd>)
                {
                    scene->getApplication()->getAssetManager<MusicManager>()->loadAsset(c.id,
                                                                                        c.path);
                }
                else if constexpr (std::is_same_v<T, PlaySoundCmd>)
                {
                    AudioPlayer::PlaySound(c.id);
                }
                else if constexpr (std::is_same_v<T, StopSoundCmd>)
                {
                    AudioPlayer::StopSound(c.id);
                }
                else if constexpr (std::is_same_v<T, PlayMusicCmd>)
                {
                    AudioPlayer::PlayMusic(c.id);
                }
                else if constexpr (std::is_same_v<T, StopMusicCmd>)
                {
                    AudioPlayer::StopMusic(c.id);
                }
                else if constexpr (std::is_same_v<T, PauseMusicCmd>)
                {
                    AudioPlayer::PauseMusic();
                }
                else if constexpr (std::is_same_v<T, ResumeMusicCmd>)
                {
                    AudioPlayer::ResumeMusic();
                }
                else if constexpr (std::is_same_v<T, CreateEntityCmd>)
                {
                    auto entity = scene->getEntityManager().createEntity();
                    auto id = static_cast<entt::entity>(entity);
                    handle_map[c.handle] = id;
                }
                else if constexpr (std::is_same_v<T, CreateWithScriptCmd>)
                {
                    auto entity = scene->getEntityManager().createEntity();
                    auto id = static_cast<entt::entity>(entity);
                    handle_map[c.handle] = id;

                    auto& script = entity.addComponent<LuaScript>(entity);
                    script.loadScriptDeferred(c.script_path);

                    for (auto& [key, val] : c.user_data)
                    {
                        script.env_[key] = val;
                    }
                }
                else if constexpr (std::is_same_v<T, DestroyEntityCmd>)
                {
                    auto ent = resolveTarget(c.entity, handle_map);
                    auto entity = scene->getEntityManager().getEntity(ent);
                    if (entity and entity.template hasComponent<ecs::LifecycleComponent>())
                    {
                        auto& t = entity.template getComponent<ecs::LifecycleComponent>();
                        t.state = ecs::EntityState::Dead;
                    }
                }
                else if constexpr (std::is_same_v<T, AddGraphicsCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent))
                    {
                        entity.template addComponent<ecs::GraphicsComponent>(c.texture_id, c.rect);
                    }
                }
                else if constexpr (std::is_same_v<T, AddTransformCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent))
                    {
                        auto& t = entity.template addComponent<ecs::TransformComponent>();
                        t.position = c.pos;
                        t.scale = c.scale;
                        t.rotation = c.rotation;
                    }
                }
                else if constexpr (std::is_same_v<T, AddFactionCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent))
                    {
                        entity.template addComponent<ecs::FactionComponent>(c.mask);
                    }
                }
                else if constexpr (std::is_same_v<T, SetPosCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent);
                        entity and entity.template hasComponent<ecs::TransformComponent>())
                    {
                        auto& t = entity.template getComponent<ecs::TransformComponent>();
                        t.position.x = c.x;
                        t.position.y = c.y;
                    }
                }
                else if constexpr (std::is_same_v<T, SetRotationCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent);
                        entity and entity.template hasComponent<ecs::TransformComponent>())
                    {
                        auto& t = entity.template getComponent<ecs::TransformComponent>();
                        t.rotation = c.angle;
                    }
                }
                else if constexpr (std::is_same_v<T, SetScaleCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent);
                        entity and entity.template hasComponent<ecs::TransformComponent>())
                    {
                        auto& t = entity.template getComponent<ecs::TransformComponent>();
                        t.scale.x = c.sx;
                        t.scale.y = c.sy;
                    }
                }
                else if constexpr (std::is_same_v<T, TriggerEventCmd>)
                {
                    auto ent = resolveTarget(c.handle, handle_map);
                    if (auto entity = scene->getEntityManager().getEntity(ent))
                    {
                        SDK_TRACE("Event triggered: {0} on entity {1}", c.event_type, entity);
                    }
                }
                else if constexpr (std::is_same_v<T, LoggerCmd>)
                {
                    switch (c.log_type)
                    {
                        case LogType::ERROR:
                            LUA_ERROR(c.log_message);
                            break;
                        case LogType::INFO:
                            LUA_INFO(c.log_message);
                            break;
                        case LogType::WARN:
                            LUA_WARN(c.log_message);
                            break;
                        case LogType::TRACE:
                            LUA_TRACE(c.log_message);
                            break;
                        case LogType::CRITICAL:
                            LUA_CRITICAL(c.log_message);
                            break;
                    }
                }
            },
            cmd);
    }
}

}  // namespace roen::lua::experimental

#endif
