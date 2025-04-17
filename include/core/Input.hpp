#ifndef ROEN_CORE_INPUT_HPP
#define ROEN_CORE_INPUT_HPP

#include <core/KeyCodes.hpp>

#include <raylib.h>

namespace roen
{

class Input
{
public:
    static bool KeyPressed(KeyCodes::Key key);
    static bool KeyReleased(KeyCodes::Key key);
    static bool KeyDown(KeyCodes::Key key);

    static KeyCodes::Key GetKey();

    static bool MouseButtonPressed(KeyCodes::MouseButton button);
    static bool MouseButtonReleased(KeyCodes::MouseButton button);
    static bool MouseButtonDown(KeyCodes::MouseButton button);
    static Vector2 MousePosition();

private:
    static int GetRaylibKey(KeyCodes::Key key);
    static int GetRaylibButton(KeyCodes::MouseButton button);
    static KeyCodes::Key GetRoenKey(int key);
    static KeyCodes::MouseButton GetRoenButton(int key);
};

}  // namespace roen

#endif  // ROEN_CORE_INPUT_HPP
