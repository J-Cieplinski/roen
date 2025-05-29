#ifndef ROEN_MATH_RECTANGLE_HPP
#define ROEN_MATH_RECTANGLE_HPP

namespace roen::math
{

// Vector2, 2 components
struct Vector2
{
    bool operator==(const Vector2& rhs) const { return x == rhs.x and y == rhs.y; }
    Vector2 operator+(Vector2 rhs) const { return Vector2{rhs.x + x, rhs.y + y}; }
    Vector2 operator-(Vector2 rhs) const { return Vector2{x - rhs.x, y - rhs.y}; }
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

}  // namespace roen::math

#endif  // ROEN_MATH_RECTANGLE_HPP
