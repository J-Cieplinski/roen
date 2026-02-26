#ifndef ROEN_LUA_SNAPSHOT_CAPTURE_HPP
#define ROEN_LUA_SNAPSHOT_CAPTURE_HPP

#include <core/Input.hpp>
#include <lua/CommandSystem.hpp>

namespace roen::lua
{

inline InputSnapshot captureInput()
{
    InputSnapshot snap;

    for (int i = 0; i < 256; i++)
    {
        auto key = static_cast<KeyCodes::Key>(i);
        snap.keys_down[i] = Input::KeyDown(key);
        snap.keys_pressed[i] = Input::KeyPressed(key);
    }

    for (int i = 0; i < 8; i++)
    {
        auto btn = static_cast<KeyCodes::MouseButton>(i);
        snap.mouse_down[i] = Input::MouseButtonDown(btn);
        snap.mouse_pressed[i] = Input::MouseButtonPressed(btn);
    }

    snap.mouse_pos = Input::MousePosition();

    return snap;
}

}  // namespace roen::lua

#endif
