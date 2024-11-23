#ifndef ROEN_DATA_STRUCTURE_VECTOR2_HPP
#define ROEN_DATA_STRUCTURE_VECTOR2_HPP

#include <cstdint>
#include <tuple>

namespace roen::data_structure
{

template <typename T>
struct Vector2
{
    T x;
    T y;

    inline bool operator==(const Vector2<T>& rhs) const = default;

    inline Vector2<T> operator+(const Vector2<T>& rhs) const
    {
        return Vector2<T>(x + rhs.x, y + rhs.y);
    }

    inline Vector2<T> operator-(const Vector2<T>& rhs) const
    {
        return Vector2<T>(x - rhs.x, y - rhs.y);
    }

    inline bool operator<(const Vector2<T>& rhs) const
    {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    inline Vector2<T> operator/(const T& scalar) const
    {
        return Vector2<T>(x / scalar, y / scalar);
    }

    inline Vector2<T> operator*(const T& scalar) const
    {
        return Vector2<T>(x * scalar, y * scalar);
    }
};

using Vector2u = Vector2<std::uint32_t>;
using Vector2i = Vector2<std::int32_t>;
using Vector2f = Vector2<float>;

} // roen::data_structure

#endif //ROEN_DATA_STRUCTURE_VECTOR2_HPP
