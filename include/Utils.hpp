#ifndef ROEN_UTILS_HPP
#define ROEN_UTILS_HPP

#include <cstdint>
#include <string>

#include <raylib.h>
#include <raymath.h>

namespace roen
{

constexpr std::int32_t pow(std::int32_t base, std::uint32_t exponent)
{
    if (exponent == 0)
    {
        return 1;
    }
    if (exponent % 2 == 0)
    {
        std::uint32_t curr = pow(base, exponent / 2);
        return curr * curr;
    }

    return base * pow(base, exponent - 1);
}

/*
 * Hashes string used for loaded asset identification
 */
constexpr std::uint64_t hashString(const std::string& str)
{
    constexpr std::uint32_t PRIME_CONST{31};
    std::uint64_t hashCode{0};
    for (std::uint32_t i = 0; const auto& ch : str)
    {
        hashCode += ch * pow(PRIME_CONST, i);
    }

    return hashCode;
}

class Functor
{
public:
    virtual ~Functor() = default;
    virtual void operator()() = 0;
};

inline Vector2 getVirtualMousePosition(std::uint32_t renderWidth, std::uint32_t renderHeight)
{
    const auto screenWidth = GetScreenWidth();
    const auto screenHeight = GetScreenHeight();
    const float scale{std::min(static_cast<float>(screenWidth) / renderWidth,
                               static_cast<float>(screenHeight) / renderHeight)};

    const auto [mouseX, mouseY] = GetMousePosition();
    const Vector2 virtualMousePosition{
        .x = (mouseX - (static_cast<float>(screenWidth) - (renderWidth * scale)) * 0.5f) / scale,
        .y = (mouseY - (static_cast<float>(screenHeight) - (renderHeight * scale)) * 0.5f) / scale,
    };

    return Vector2Clamp(virtualMousePosition, Vector2Zero(),
                        Vector2{static_cast<float>(renderWidth), static_cast<float>(renderHeight)});
}

#ifdef __GNUC__
}  // roen

#    include <cxxabi.h>

namespace roen
{

inline std::string getDemangledName(const std::string& name)
{
    char* demangledName = nullptr;
    demangledName = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, nullptr);
    const std::string result{demangledName};
    free(demangledName);

    return result;
}

#endif

}  // roen

#endif  // ROEN_UTILS_HPP
