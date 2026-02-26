#ifndef ROEN_LUA_API_BINDINGS_HPP
#define ROEN_LUA_API_BINDINGS_HPP

#include <lua/CommandSystem.hpp>
#include <lua/ReadOnlyECS.hpp>

#include <sol2/sol.hpp>

namespace roen::lua
{

inline Target make_target(sol::object const& obj)
{
    if (obj.is<entt::entity>())
    {
        return obj.as<entt::entity>();
    }
    if (obj.is<Handle>())
    {
        return obj.as<Handle>();
    }

    return entt::entity{entt::null};
}

inline void bindAssetApi(sol::state& lua)
{
    auto assets = lua["Assets"].get_or_create<sol::table>();

    auto asset_func = [](sol::this_environment te, auto cmd)
    {
        sol::environment env{te};
        if (CommandBuffer* cmd_buf = env["_cmd_buf"])
        {
            cmd_buf->push(cmd);
        }
    };

    assets.set_function(
        "loadTexture", [&asset_func](std::string id, std::string path, sol::this_environment te)
        { asset_func(std::move(te), LoadTextureCmd{std::move(id), std::move(path)}); });

    assets.set_function(
        "loadSound", [&asset_func](std::string id, std::string path, sol::this_environment te)
        { asset_func(std::move(te), LoadSoundCmd{std::move(id), std::move(path)}); });

    assets.set_function(
        "loadMusic", [&asset_func](std::string id, std::string path, sol::this_environment te)
        { asset_func(std::move(te), LoadMusicCmd{std::move(id), std::move(path)}); });
}

inline void bindAudioApi(sol::state& lua)
{
    auto audio = lua["Audio"].get_or_create<sol::table>();

    auto audio_func = [](sol::this_environment te, auto cmd)
    {
        sol::environment env{te};
        if (CommandBuffer* cmd_buf = env["_cmd_buf"])
        {
            cmd_buf->push(cmd);
        }
    };

    audio.set_function("playSound", [&audio_func](std::string id, sol::this_environment te)
                       { audio_func(te, PlaySoundCmd{std::move(id)}); });

    audio.set_function("stopSound", [&audio_func](std::string id, sol::this_environment te)
                       { audio_func(te, StopSoundCmd{std::move(id)}); });

    audio.set_function("playMusic", [&audio_func](std::string id, sol::this_environment te)
                       { audio_func(te, PlayMusicCmd{std::move(id)}); });

    audio.set_function("stopMusic", [&audio_func](std::string id, sol::this_environment te)
                       { audio_func(te, StopMusicCmd{std::move(id)}); });
}

inline void bindEcsApi(sol::state& lua)
{
    auto ecs = lua["ECS"].get_or_create<sol::table>();

    auto create_ecs_func = [](sol::this_environment te, auto cmd_factory) -> Handle
    {
        sol::environment env{te};
        CommandBuffer* cmd_buf = env["_cmd_buf"];
        if (!cmd_buf) return 0;
        Handle h = cmd_buf->allocHandle();
        cmd_buf->push(cmd_factory(h));
        return h;
    };

    ecs.set_function("create", [&create_ecs_func](sol::this_environment te) -> Handle
                     { return create_ecs_func(te, [](Handle h) { return CreateEntityCmd{h}; }); });

    ecs.set_function("createWithScript",
                     [&create_ecs_func](std::string script_path, sol::this_environment te) -> Handle
                     {
                         return create_ecs_func(
                             te, [&script_path](Handle h)
                             { return CreateWithScriptCmd{h, std::move(script_path), {}}; });
                     });

    ecs.set_function("destroy",
                     [](sol::object e_like, sol::this_environment te)
                     {
                         sol::environment env{te};
                         if (CommandBuffer* cmd_buf = env["_cmd_buf"])
                         {
                             cmd_buf->push(DestroyEntityCmd{make_target(e_like)});
                         }
                     });

    auto ecs_func = [](sol::this_environment te, auto cmd)
    {
        sol::environment env{te};
        CommandBuffer* cmd_buf = env["_cmd_buf"];
        if (cmd_buf) cmd_buf->push(cmd);
    };

    ecs.set_function("setGraphics", [&ecs_func](sol::object e_like, std::string tex,
                                                math::Rectangle rect, sol::this_environment te)
                     { ecs_func(te, AddGraphicsCmd{make_target(e_like), std::move(tex), rect}); });

    ecs.set_function("addTransform", [&ecs_func](sol::object e_like, sol::this_environment te)
                     { ecs_func(te, AddTransformCmd{make_target(e_like)}); });

    ecs.set_function("setPos",
                     [&ecs_func](sol::object e_like, float x, float y, sol::this_environment te)
                     { ecs_func(te, SetPosCmd{make_target(e_like), x, y}); });

    ecs.set_function("setRotation",
                     [&ecs_func](sol::object e_like, float angle, sol::this_environment te)
                     { ecs_func(te, SetRotationCmd{make_target(e_like), angle}); });

    ecs.set_function("setScale",
                     [&ecs_func](sol::object e_like, float sx, float sy, sol::this_environment te)
                     { ecs_func(te, SetScaleCmd{make_target(e_like), sx, sy}); });
}

void inline bindMathApi(sol::state& lua)
{
    auto vec2 = lua.new_usertype<math::Vector2>(
        "Vector2", sol::constructors<math::Vector2(float, float)>(), "x", &math::Vector2::x, "y",
        &math::Vector2::y, sol::meta_function::addition, &math::Vector2::operator+,
        sol::meta_function::subtraction, &math::Vector2::operator-,
        sol::meta_function::multiplication, &math::Vector2::operator*, sol::meta_function::equal_to,
        &math::Vector2::operator==);

    auto rect = lua.new_usertype<math::Rectangle>(
        "Rectangle", sol::constructors<math::Rectangle(float, float, float, float)>(), "x",
        &math::Rectangle::x, "y", &math::Rectangle::y, "width", &math::Rectangle::width, "height",
        &math::Rectangle::height, sol::meta_function::equal_to, &math::Rectangle::operator==);
}

inline void bindLoggerApi(sol::state& lua)
{
    auto logger = lua["Logger"].get_or_create<sol::table>();

    auto bindLogger = [](sol::object obj, sol::this_environment te, LogType logType)
    {
        sol::environment env{te};
        sol::state_view lua_view = obj.lua_state();
        std::string msg = lua_view["tostring"](obj);
        if (CommandBuffer* cmd_buf = env["_cmd_buf"])
        {
            cmd_buf->push(LoggerCmd{std::move(msg), logType});
        }
    };

    logger.set_function("ERROR", [&bindLogger](sol::object obj, sol::this_environment te)
                        { bindLogger(std::move(obj), std::move(te), LogType::ERROR); });

    logger.set_function("INFO", [&bindLogger](sol::object obj, sol::this_environment te)
                        { bindLogger(std::move(obj), std::move(te), LogType::INFO); });

    logger.set_function("WARN", [&bindLogger](sol::object obj, sol::this_environment te)
                        { bindLogger(std::move(obj), std::move(te), LogType::WARN); });

    logger.set_function("TRACE", [&bindLogger](sol::object obj, sol::this_environment te)
                        { bindLogger(std::move(obj), std::move(te), LogType::TRACE); });

    logger.set_function("CRITICAL", [&bindLogger](sol::object obj, sol::this_environment te)
                        { bindLogger(std::move(obj), std::move(te), LogType::CRITICAL); });
}

inline void bindButtons(sol::state& lua)
{
    std::initializer_list<std::pair<sol::string_view, KeyCodes::Key>> keyItems
        = {{"NONE", KeyCodes::Key::NONE},
           {"APOSTROPHE", KeyCodes::Key::APOSTROPHE},
           {"COMMA", KeyCodes::Key::COMMA},
           {"MINUS", KeyCodes::Key::MINUS},
           {"PERIOD", KeyCodes::Key::PERIOD},
           {"SLASH", KeyCodes::Key::SLASH},
           {"ZERO", KeyCodes::Key::ZERO},
           {"ONE", KeyCodes::Key::ONE},
           {"TWO", KeyCodes::Key::TWO},
           {"THREE", KeyCodes::Key::THREE},
           {"FOUR", KeyCodes::Key::FOUR},
           {"FIVE", KeyCodes::Key::FIVE},
           {"SIX", KeyCodes::Key::SIX},
           {"SEVEN", KeyCodes::Key::SEVEN},
           {"EIGHT", KeyCodes::Key::EIGHT},
           {"NINE", KeyCodes::Key::NINE},
           {"SEMICOLON", KeyCodes::Key::SEMICOLON},
           {"EQUAL", KeyCodes::Key::EQUAL},
           {"A", KeyCodes::Key::A},
           {"B", KeyCodes::Key::B},
           {"C", KeyCodes::Key::C},
           {"D", KeyCodes::Key::D},
           {"E", KeyCodes::Key::E},
           {"F", KeyCodes::Key::F},
           {"G", KeyCodes::Key::G},
           {"H", KeyCodes::Key::H},
           {"I", KeyCodes::Key::I},
           {"J", KeyCodes::Key::J},
           {"K", KeyCodes::Key::K},
           {"L", KeyCodes::Key::L},
           {"M", KeyCodes::Key::M},
           {"N", KeyCodes::Key::N},
           {"O", KeyCodes::Key::O},
           {"P", KeyCodes::Key::P},
           {"Q", KeyCodes::Key::Q},
           {"R", KeyCodes::Key::R},
           {"S", KeyCodes::Key::S},
           {"T", KeyCodes::Key::T},
           {"U", KeyCodes::Key::U},
           {"V", KeyCodes::Key::V},
           {"W", KeyCodes::Key::W},
           {"X", KeyCodes::Key::X},
           {"Y", KeyCodes::Key::Y},
           {"Z", KeyCodes::Key::Z},
           {"LEFT_BRACKET", KeyCodes::Key::LEFT_BRACKET},
           {"BACKSLASH", KeyCodes::Key::BACKSLASH},
           {"RIGHT_BRACKET", KeyCodes::Key::RIGHT_BRACKET},
           {"GRAVE", KeyCodes::Key::GRAVE},
           {"SPACE", KeyCodes::Key::SPACE},
           {"ESCAPE", KeyCodes::Key::ESCAPE},
           {"ENTER", KeyCodes::Key::ENTER},
           {"TAB", KeyCodes::Key::TAB},
           {"BACKSPACE", KeyCodes::Key::BACKSPACE},
           {"INSERT", KeyCodes::Key::INSERT},
           {"DELETE", KeyCodes::Key::DELETE},
           {"RIGHT", KeyCodes::Key::RIGHT},
           {"LEFT", KeyCodes::Key::LEFT},
           {"DOWN", KeyCodes::Key::DOWN},
           {"UP", KeyCodes::Key::UP},
           {"PAGE_UP", KeyCodes::Key::PAGE_UP},
           {"PAGE_DOWN", KeyCodes::Key::PAGE_DOWN},
           {"HOME", KeyCodes::Key::HOME},
           {"END", KeyCodes::Key::END},
           {"CAPS_LOCK", KeyCodes::Key::CAPS_LOCK},
           {"SCROLL_LOCK", KeyCodes::Key::SCROLL_LOCK},
           {"NUM_LOCK", KeyCodes::Key::NUM_LOCK},
           {"PRINT_SCREEN", KeyCodes::Key::PRINT_SCREEN},
           {"PAUSE", KeyCodes::Key::PAUSE},
           {"F1", KeyCodes::Key::F1},
           {"F2", KeyCodes::Key::F2},
           {"F3", KeyCodes::Key::F3},
           {"F4", KeyCodes::Key::F4},
           {"F5", KeyCodes::Key::F5},
           {"F6", KeyCodes::Key::F6},
           {"F7", KeyCodes::Key::F7},
           {"F8", KeyCodes::Key::F8},
           {"F9", KeyCodes::Key::F9},
           {"F10", KeyCodes::Key::F10},
           {"F11", KeyCodes::Key::F11},
           {"F12", KeyCodes::Key::F12},
           {"LEFT_SHIFT", KeyCodes::Key::LEFT_SHIFT},
           {"LEFT_CONTROL", KeyCodes::Key::LEFT_CONTROL},
           {"LEFT_ALT", KeyCodes::Key::LEFT_ALT},
           {"LEFT_SUPER", KeyCodes::Key::LEFT_SUPER},
           {"RIGHT_SHIFT", KeyCodes::Key::RIGHT_SHIFT},
           {"RIGHT_CONTROL", KeyCodes::Key::RIGHT_CONTROL},
           {"RIGHT_ALT", KeyCodes::Key::RIGHT_ALT},
           {"RIGHT_SUPER", KeyCodes::Key::RIGHT_SUPER},
           {"KB_MENU", KeyCodes::Key::KB_MENU},
           {"KP_0", KeyCodes::Key::KP_0},
           {"KP_1", KeyCodes::Key::KP_1},
           {"KP_2", KeyCodes::Key::KP_2},
           {"KP_3", KeyCodes::Key::KP_3},
           {"KP_4", KeyCodes::Key::KP_4},
           {"KP_5", KeyCodes::Key::KP_5},
           {"KP_6", KeyCodes::Key::KP_6},
           {"KP_7", KeyCodes::Key::KP_7},
           {"KP_8", KeyCodes::Key::KP_8},
           {"KP_9", KeyCodes::Key::KP_9},
           {"KP_DECIMAL", KeyCodes::Key::KP_DECIMAL},
           {"KP_DIVIDE", KeyCodes::Key::KP_DIVIDE},
           {"KP_MULTIPLY", KeyCodes::Key::KP_MULTIPLY},
           {"KP_SUBTRACT", KeyCodes::Key::KP_SUBTRACT},
           {"KP_ADD", KeyCodes::Key::KP_ADD},
           {"KP_ENTER", KeyCodes::Key::KP_ENTER},
           {"KP_EQUAL", KeyCodes::Key::KP_EQUAL},
           {"BACK", KeyCodes::Key::BACK},
           {"MENU", KeyCodes::Key::MENU},
           {"VOLUME_UP", KeyCodes::Key::VOLUME_UP},
           {"VOLUME_DOWN", KeyCodes::Key::VOLUME_DOWN}};
    lua.new_enum<KeyCodes::Key>("Key", keyItems);

    std::initializer_list<std::pair<sol::string_view, KeyCodes::MouseButton>> mouseItems
        = {{"LEFT", KeyCodes::MouseButton::LEFT},     {"RIGHT", KeyCodes::MouseButton::RIGHT},
           {"MIDDLE", KeyCodes::MouseButton::MIDDLE}, {"FORWARD", KeyCodes::MouseButton::FORWARD},
           {"BACK", KeyCodes::MouseButton::BACK},     {"EXTRA", KeyCodes::MouseButton::EXTRA},
           {"SIDE", KeyCodes::MouseButton::SIDE}};
    lua.new_enum<KeyCodes::MouseButton>("MouseButton", mouseItems);
}

inline void bindGlobalAPI(sol::state& lua)
{
    bindAssetApi(lua);

    bindAudioApi(lua);

    bindEcsApi(lua);

    bindMathApi(lua);

    bindLoggerApi(lua);

    bindButtons(lua);

    auto events = lua["Events"].get_or_create<sol::table>();

    events.set_function("trigger",
                        [](Handle target, std::string type, sol::this_environment te)
                        {
                            sol::environment env{te};
                            CommandBuffer* cmd_buf = env["_cmd_buf"];
                            if (cmd_buf)
                                cmd_buf->push(TriggerEventCmd{target, std::move(type), {}});
                        });

    lua.new_usertype<InputSnapshot>(
        "InputSnapshot", sol::no_constructor, "keyDown", &InputSnapshot::keyDown, "keyPressed",
        &InputSnapshot::keyPressed, "mouseDown", &InputSnapshot::mouseDown, "mousePressed",
        &InputSnapshot::mousePressed, "mousePos", &InputSnapshot::mousePos);

    lua.new_usertype<EntityView>(
        "EntityView", sol::no_constructor, "id", sol::property(&EntityView::getId), "isValid",
        &EntityView::isValid, "hasTransform", &EntityView::hasTransform, "hasGraphics",
        &EntityView::hasGraphics, "hasFaction", &EntityView::hasFaction, "getPos",
        &EntityView::getPos, "getRotation", &EntityView::getRotation, "getScale",
        &EntityView::getScale, "getTexture", &EntityView::getTexture, "getSrcRect",
        &EntityView::getSrcRect, "getFactionMask", &EntityView::getFactionMask);

    lua.new_usertype<SceneQuery>("SceneQuery", sol::no_constructor, "getEntity",
                                 &SceneQuery::getEntity, "exists", &SceneQuery::exists);
}

}  // namespace roen::lua

#endif
