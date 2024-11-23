#ifndef ROEN_LOG_FORMATTERS_ENTITY_HPP
#define ROEN_LOG_FORMATTERS_ENTITY_HPP

#include <fmt/core.h>
#include <fmt/format.h>

#include <entt/fwd.hpp>

namespace fmt
{

template <>
struct formatter<entt::entity> : formatter<string_view>
{
public:
    inline auto format(entt::entity entt, format_context& ctx) const;
};

inline auto formatter<entt::entity>::format(entt::entity entity, format_context& ctx) const
{
    string_view enttStr {std::to_string(static_cast<std::uint32_t>(entity))};

    return formatter<string_view>::format(enttStr, ctx);
}

} // namespace fmt

#endif //ROEN_LOG_FORMATTERS_ENTITY_HPP