#ifndef ROEN_MATH_RECTANGLE_HPP
#define ROEN_MATH_RECTANGLE_HPP

#include <expected>
#include <numbers>

namespace roen::math
{

inline constexpr auto TO_RADIANS{std::numbers::pi / 180.f};
inline constexpr auto TO_DEGREES{180.f / std::numbers::pi};

// Vector2, 2 components
struct Vector2
{
    bool operator==(const Vector2& rhs) const { return x == rhs.x and y == rhs.y; }
    Vector2 operator+(Vector2 rhs) const { return Vector2{rhs.x + x, rhs.y + y}; }
    Vector2 operator-(Vector2 rhs) const { return Vector2{x - rhs.x, y - rhs.y}; }
    Vector2 operator*(Vector2 rhs) const { return Vector2{x * rhs.x, y * rhs.y}; }
    float x{0};  // Vector x component
    float y{0};  // Vector y component
};

// Vector3, 3 components
struct Vector3
{
    bool operator==(const Vector3& rhs) const { return x == rhs.x and y == rhs.y and z == rhs.z; }
    Vector3 operator+(Vector3 rhs) const { return Vector3{rhs.x + x, rhs.y + y, rhs.z + z}; }
    Vector3 operator-(Vector3 rhs) const { return Vector3{x - rhs.x, y - rhs.y, z - rhs.z}; }
    float x{0};  // Vector x component
    float y{0};  // Vector y component
    float z{0};  // Vector z component
};

// Color, 4 components, R8G8B8A8 (32bit)
struct Color
{
    unsigned char r;  // Color red value
    unsigned char g;  // Color green value
    unsigned char b;  // Color blue value
    unsigned char a;  // Color alpha value
};

// Rectangle, 4 components
struct Rectangle
{
    bool operator==(const Rectangle& rhs) const
    {
        return x == rhs.x and y == rhs.y and width == rhs.width and height == rhs.height;
    }

    float x{0};       // Rectangle top-left corner position x
    float y{0};       // Rectangle top-left corner position y
    float width{0};   // Rectangle width
    float height{0};  // Rectangle height
};

enum class MatrixAccessError
{
    OUT_OF_BOUNDS_ACCESS_ATTEMPT
};

struct Matrix2D
{
    std::array<float, 6> m{1, 0, 0, 1, 0, 0};  // a,b,c,d,tx,ty in 2x3 matrix form

    std::expected<float, MatrixAccessError> operator[](std::size_t index) const
    {
        if (index >= m.size())
        {
            SDK_ERROR("Attempted to access non-existant matrix element");
            return std::unexpected{MatrixAccessError::OUT_OF_BOUNDS_ACCESS_ATTEMPT};
        }
        return m[index];
    }

    std::expected<std::reference_wrapper<float>, MatrixAccessError> operator[](std::size_t index)
    {
        if (index >= m.size())
        {
            SDK_ERROR("Attempted to access non-existant matrix element");
            return std::unexpected{MatrixAccessError::OUT_OF_BOUNDS_ACCESS_ATTEMPT};
        }
        return m[index];
    }

    static Matrix2D Identity() { return Matrix2D{}; }

    // Compose matrix from position, rotation (radians), and scale
    void compose(const Vector2& position, float rotation, const Vector2& scale)
    {
        const float cosR = std::cos(rotation);
        const float sinR = std::sin(rotation);

        m[0] = cosR * scale.x;   // a
        m[1] = sinR * scale.x;   // b
        m[2] = -sinR * scale.y;  // c
        m[3] = cosR * scale.y;   // d
        m[4] = position.x;       // tx
        m[5] = position.y;       // ty
    }

    // Multiply this matrix by a parent matrix (this = this * parent)
    void applyParent(const Matrix2D& parent)
    {
        float a = m[0], b = m[1], c = m[2], d = m[3], tx = m[4], ty = m[5];

        m[0] = a * parent.m[0] + c * parent.m[1];
        m[1] = b * parent.m[0] + d * parent.m[1];
        m[2] = a * parent.m[2] + c * parent.m[3];
        m[3] = b * parent.m[2] + d * parent.m[3];
        m[4] = a * parent.m[4] + c * parent.m[5] + tx;
        m[5] = b * parent.m[4] + d * parent.m[5] + ty;
    }

    // Extract position from matrix
    Vector2 getPosition() const { return {m[4], m[5]}; }

    // Extract scale from matrix
    Vector2 getScale() const
    {
        return {std::sqrt(m[0] * m[0] + m[1] * m[1]), std::sqrt(m[2] * m[2] + m[3] * m[3])};
    }

    // Extract rotation in radians from matrix
    float getRotation() const { return std::atan2(m[1], m[0]); }
};

}  // namespace roen::math

#endif  // ROEN_MATH_RECTANGLE_HPP
