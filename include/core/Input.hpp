#ifndef ROEN_CORE_INPUT_HPP
#define ROEN_CORE_INPUT_HPP

#include <core/KeyCodes.hpp>

#include <math/Types.hpp>

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
    static math::Vector2 MousePosition();

private:
    // TODO: this can be abstracted to implementation with raylib code
    static int GetRaylibKey(KeyCodes::Key key);
    static int GetRaylibButton(KeyCodes::MouseButton button);
    static math::Vector2 GetRaylibMousePosition();
    static KeyCodes::Key GetRoenKey(int key);
    static KeyCodes::MouseButton GetRoenButton(int key);
};

}  // namespace roen

#endif  // ROEN_CORE_INPUT_HPP
