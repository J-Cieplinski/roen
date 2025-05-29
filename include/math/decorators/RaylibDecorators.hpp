#ifndef ROEN_MATH_DECORATORS_RAYLIB_DECORATORS_HPP
#define ROEN_MATH_DECORATORS_RAYLIB_DECORATORS_HPP

#include <math/Types.hpp>

#include <raymath.h>

struct RaylibVector2
{
    RaylibVector2(roen::math::Vector2 vec)
        : data{vec}
    {
    }

    RaylibVector2(::Vector2 vec)
        : data{vec.x, vec.y}
    {
    }

    operator Vector2() const { return {data.x, data.y}; }
    operator roen::math::Vector2() const { return data; }
    Vector2 operator+(roen::math::Vector2 rhs) const { return {data.x + rhs.x, data.y + rhs.y}; }
    roen::math::Vector2 data;
};

struct RaylibRectangle
{
    RaylibRectangle(roen::math::Rectangle rec)
        : data{rec}
    {
    }

    RaylibRectangle(::Rectangle rec)
        : data{rec.x, rec.y, rec.width, rec.height}
    {
    }

    operator Rectangle() const { return {data.x, data.y, data.width, data.height}; }
    operator roen::math::Rectangle() const { return data; }
    roen::math::Rectangle data;
};

#endif  // ROEN_MATH_DECORATORS_RAYLIB_DECORATORS_HPP
