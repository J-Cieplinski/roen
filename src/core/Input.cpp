#include <core/Input.hpp>

namespace roen
{

bool Input::KeyPressed(KeyCodes::Key key)
{
    return IsKeyPressed(GetRaylibKey(key));
}

bool Input::KeyReleased(KeyCodes::Key key)
{
    return IsKeyReleased(GetRaylibKey(key));
}

bool Input::KeyDown(KeyCodes::Key key)
{
    return IsKeyDown(GetRaylibKey(key));
}

KeyCodes::Key Input::GetKey()
{
    return GetRoenKey(GetKeyPressed());
}

bool Input::MouseButtonPressed(KeyCodes::MouseButton button)
{
    return IsMouseButtonPressed(GetRaylibButton(button));
}

bool Input::MouseButtonReleased(KeyCodes::MouseButton button)
{
    return IsMouseButtonReleased(GetRaylibButton(button));
}

bool Input::MouseButtonDown(KeyCodes::MouseButton button)
{
    return IsMouseButtonDown(GetRaylibButton(button));
}

Vector2 Input::MousePosition()
{
    return GetMousePosition();
}

int Input::GetRaylibKey(KeyCodes::Key key)
{
    return static_cast<int>(key);
}

int Input::GetRaylibButton(KeyCodes::MouseButton button)
{
    return static_cast<int>(button);
}

KeyCodes::Key Input::GetRoenKey(int key)
{
    return static_cast<KeyCodes::Key>(key);
}

KeyCodes::MouseButton Input::GetRoenButton(int button)
{
    return static_cast<KeyCodes::MouseButton>(button);
}

}  // namespace roen
