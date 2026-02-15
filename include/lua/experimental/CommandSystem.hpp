#ifndef ROEN_LUA_COMMAND_SYSTEM_HPP
#define ROEN_LUA_COMMAND_SYSTEM_HPP

#include <core/KeyCodes.hpp>
#include <math/Types.hpp>

#include <bitset>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace roen::lua::experimental
{

using Handle = std::uint32_t;

using Target = std::variant<entt::entity, Handle>;

struct LoadTextureCmd
{
    std::string id, path;
};

struct LoadSoundCmd
{
    std::string id, path;
};

struct LoadMusicCmd
{
    std::string id, path;
};

struct PlaySoundCmd
{
    std::string id;
};

struct StopSoundCmd
{
    std::string id;
};

struct PlayMusicCmd
{
    std::string id;
};

struct StopMusicCmd
{
    std::string id;
};

struct PauseMusicCmd
{
};

struct ResumeMusicCmd
{
};

struct CreateEntityCmd
{
    Handle handle;
};

struct CreateWithScriptCmd
{
    Handle handle;
    std::string script_path;
    std::unordered_map<std::string, std::string> user_data;
};

struct DestroyEntityCmd
{
    Target entity;
};

struct AddGraphicsCmd
{
    Target handle;
    std::string texture_id;
    math::Rectangle rect;
};

struct AddTransformCmd
{
    Target handle;
    math::Vector2 pos{0, 0};
    math::Vector2 scale{1, 1};
    float rotation = 0;
};

struct AddFactionCmd
{
    Target handle;
    std::uint8_t mask;
};

struct SetPosCmd
{
    Target handle;
    float x, y;
};

struct SetRotationCmd
{
    Target handle;
    float angle;
};

struct SetScaleCmd
{
    Target handle;
    float sx, sy;
};

struct TriggerEventCmd
{
    Target handle;
    std::string event_type;
    std::unordered_map<std::string, std::string> data;
};

enum class LogType
{
    ERROR,
    INFO,
    WARN,
    TRACE,
    CRITICAL,
};

struct LoggerCmd
{
    std::string log_message;
    LogType log_type;
};

using Command = std::variant<LoadTextureCmd, LoadSoundCmd, LoadMusicCmd, PlaySoundCmd, StopSoundCmd,
                             PlayMusicCmd, StopMusicCmd, PauseMusicCmd, ResumeMusicCmd,
                             CreateEntityCmd, CreateWithScriptCmd, DestroyEntityCmd, AddGraphicsCmd,
                             AddTransformCmd, AddFactionCmd, SetPosCmd, SetRotationCmd, SetScaleCmd,
                             TriggerEventCmd, LoggerCmd>;

struct CommandBuffer
{
    std::vector<Command> commands;
    Handle next_handle = 1;

    void push(Command cmd) { commands.push_back(std::move(cmd)); }

    Handle allocHandle() { return next_handle++; }
};

// ========== Input Snapshot ==========
struct InputSnapshot
{
    std::bitset<256> keys_down;
    std::bitset<256> keys_pressed;
    std::bitset<8> mouse_down;
    std::bitset<8> mouse_pressed;
    math::Vector2 mouse_pos;

    bool keyDown(KeyCodes::Key k) const { return keys_down[static_cast<int>(k)]; }
    bool keyPressed(KeyCodes::Key k) const { return keys_pressed[static_cast<int>(k)]; }
    bool mouseDown(KeyCodes::MouseButton b) const { return mouse_down[static_cast<int>(b)]; }
    bool mousePressed(KeyCodes::MouseButton b) const { return mouse_pressed[static_cast<int>(b)]; }
    math::Vector2 mousePos() const { return mouse_pos; }
};

}  // namespace roen::lua::experimental

#endif
