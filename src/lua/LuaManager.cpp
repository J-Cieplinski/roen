#include <lua/LuaManager.hpp>

#include <lua/scripts/LuaEventHandler.hpp>

#include <Application.hpp>

#include <core/AssetManager.hpp>
#include <core/AudioPlayer.hpp>
#include <core/Input.hpp>
#include <core/KeyCodes.hpp>

#include <ecs/Entity.hpp>
#include <ecs/EntityManager.hpp>

#include <ecs/components/FactionComponent.hpp>
#include <ecs/components/GraphicsComponent.hpp>
#include <ecs/components/TransformComponent.hpp>

#include <events/Event.hpp>
#include <events/KeyEvent.hpp>
#include <events/MouseEvent.hpp>

#include <interfaces/Scene.hpp>

#include <log/Logger.hpp>

#include <Utils.hpp>

#include "core/AudioPlayer.hpp"

namespace roen::lua
{
#define STR(str) #str
#define CONCAT(left, right) STR(left##right)

#define REGISTER_COMPONENT_WITH_ECS(curLuaState, Comp, ...)                                       \
    {                                                                                             \
        namespace component = ecs::components;                                                    \
        auto entity_type = curLuaState["Entity"].get_or_create<sol::usertype<ecs::Entity>>();     \
        entity_type.set_function(CONCAT(remove, Comp),                                            \
                                 &ecs::Entity::removeComponent<component::Comp>);                 \
        entity_type.set_function(CONCAT(get, Comp), [](ecs::Entity& self) -> component::Comp&     \
                                 { return std::ref(self.getComponent<component::Comp>()); });     \
        entity_type.set_function(CONCAT(add, Comp), &ecs::Entity::addComponent<component::Comp>); \
        entity_type.set_function(CONCAT(add, Comp), sol::overload(__VA_ARGS__));                  \
        entity_type.set_function(CONCAT(has, Comp), &ecs::Entity::hasComponent<component::Comp>); \
    }

LuaManager::~LuaManager()
{
    instance_.reset();
}

LuaManager& LuaManager::Instance()
{
    if (not instance_)
    {
        instance_ = std::unique_ptr<LuaManager>(new LuaManager());

        lua_gc(instance_->lua_, LUA_GCSETPAUSE, 0);  // collect as often as possible
        lua_gc(instance_->lua_, LUA_GCSETSTEPMUL, 1);
    }
    return *instance_;
}

void LuaManager::onInit(interfaces::Scene* scene)
{
    scene_ = scene;
    InitLua();
    InitLuaApplication();
    InitLuaInput();
    InitLuaAssets();
    InitLuaLog();
    InitEventTypes();
    InitECS();
    InitScene();
    InitMathTypes();
    InitLuaEventHandler();
    InitUtils();
}

void LuaManager::InitLua()
{
    instance_->lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
                                   sol::lib::table, sol::lib::os, sol::lib::string);
}

void LuaManager::InitLuaApplication()
{
    auto application = instance_->lua_.new_usertype<Application>("Application");
}

#define SET_ASSET_MANAGER_FUNC(name)                                                             \
    {                                                                                            \
        auto applicationType                                                                     \
            = instance_->lua_["Application"].get_or_create<sol::usertype<Application>>();        \
        applicationType.set_function(                                                            \
            CONCAT(get, name), [](Application* self) { return self->getAssetManager<name>(); }); \
        auto name##Usertype = instance_->lua_.new_usertype<name>(#name, sol::no_constructor);    \
        name##Usertype.set_function("loadAsset",                                                 \
                                    [](std::shared_ptr<name> self, const std::string& id,        \
                                       const std::string& path) { self->loadAsset(id, path); }); \
        name##Usertype.set_function("getAsset", [](name& self, const std::string& id)            \
                                    { return std::ref(self.getAsset(id)); });                    \
    }

void LuaManager::InitLuaAssets()
{
    SET_ASSET_MANAGER_FUNC(SoundManager)
    SET_ASSET_MANAGER_FUNC(TextureManager)
    SET_ASSET_MANAGER_FUNC(FontManager)
    SET_ASSET_MANAGER_FUNC(MusicManager)

    auto audioPlayer = instance_->lua_["AudioPlayer"].get_or_create<sol::table>();
    audioPlayer.set_function("PlaySound",
                             [](std::string_view sound) { AudioPlayer::PlaySound(sound); });
    audioPlayer.set_function("StopSound",
                             [](std::string_view sound) { AudioPlayer::StopSound(sound); });
    audioPlayer.set_function("PlayMusic",
                             [](std::string_view music) { AudioPlayer::PlayMusic(music); });
    audioPlayer.set_function("StopMusic",
                             [](std::string_view music) { AudioPlayer::StopMusic(music); });
    audioPlayer.set_function("PauseMusic", [] { AudioPlayer::PauseMusic(); });
    audioPlayer.set_function("ResumeMusic", [] { AudioPlayer::ResumeMusic(); });
}

void LuaManager::InitLuaInput()
{
    auto input = instance_->lua_["Input"].get_or_create<sol::table>();

    input.set_function("KeyPressed",
                       [](KeyCodes::Key key) -> bool { return Input::KeyPressed(key); });

    input.set_function("KeyReleased",
                       [](KeyCodes::Key key) -> bool { return Input::KeyReleased(key); });

    input.set_function("KeyDown", [](KeyCodes::Key key) -> bool { return Input::KeyDown(key); });

    input.set_function("GetKey", []() -> KeyCodes::Key { return Input::GetKey(); });

    input.set_function("MouseButtonPressed", [](KeyCodes::MouseButton button) -> bool
                       { return Input::MouseButtonPressed(button); });

    input.set_function("MouseButtonReleased", [](KeyCodes::MouseButton button) -> bool
                       { return Input::MouseButtonReleased(button); });

    input.set_function("MouseButtonDown", [](KeyCodes::MouseButton button) -> bool
                       { return Input::MouseButtonDown(button); });

    input.set_function("MousePosition", []() -> math::Vector2 { return Input::MousePosition(); });

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

    instance_->lua_.new_enum<KeyCodes::Key, false>("Key", keyItems);

    std::initializer_list<std::pair<sol::string_view, KeyCodes::MouseButton>> mouseItems = {
        {"LEFT", KeyCodes::MouseButton::LEFT},     {"RIGHT", KeyCodes::MouseButton::RIGHT},
        {"MIDDLE", KeyCodes::MouseButton::MIDDLE}, {"FORWARD", KeyCodes::MouseButton::FORWARD},
        {"BACK", KeyCodes::MouseButton::BACK},     {"EXTRA", KeyCodes::MouseButton::EXTRA},
        {"SIDE", KeyCodes::MouseButton::SIDE},
    };

    instance_->lua_.new_enum<KeyCodes::MouseButton, false>("MouseButton", mouseItems);
}

#ifdef IS_DEBUG
#    define SET_LOGGER_FUNC(level, name)                                       \
        logger.set_function(#name,                                             \
                            [&state](sol::variadic_args va) -> void            \
                            {                                                  \
                                std::string result;                            \
                                sol::function luaToString = state["tostring"]; \
                                for (auto&& arg : va)                          \
                                {                                              \
                                    sol::object str = luaToString(arg);        \
                                    result += str.as<std::string>() + " ";     \
                                }                                              \
                                                                               \
                                level(result);                                 \
                            });
#else
#    define SET_LOGGER_FUNC(level, name) \
        logger.set_function(#name, [](sol::variadic_args va) { level(va); });
#endif  // IS_DEBUG

void LuaManager::InitLuaLog()
{
    auto logger = instance_->lua_["Logger"].get_or_create<sol::table>();
    auto& state = instance_->lua_;

    SET_LOGGER_FUNC(LUA_ERROR, Error)
    SET_LOGGER_FUNC(LUA_INFO, Info)
    SET_LOGGER_FUNC(LUA_WARN, Warn)
    SET_LOGGER_FUNC(LUA_TRACE, Trace)
    SET_LOGGER_FUNC(LUA_CRITICAL, Critical)
}

void LuaManager::InitMathTypes()
{
    auto vec2 = instance_->lua_.new_usertype<math::Vector2>(
        "Vector2", sol::constructors<math::Vector2(float, float)>(), sol::meta_function::to_string,
        [](const math::Vector2& vec) -> std::string
        { return "{x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + "}"; },
        sol::meta_function::addition, &math::Vector2::operator+, sol::meta_function::subtraction,
        &math::Vector2::operator-, sol::meta_function::equal_to, &math::Vector2::operator==);

    vec2["x"] = &math::Vector2::x;
    vec2["y"] = &math::Vector2::y;

    auto rec = instance_->lua_.new_usertype<math::Rectangle>(
        "Rectangle", sol::constructors<math::Rectangle(float, float, float, float)>(),
        sol::meta_function::to_string,
        [](const math::Rectangle& rectangle) -> std::string
        {
            return "{x: " + std::to_string(rectangle.x) + " y: " + std::to_string(rectangle.y)
                   + " width: " + std::to_string(rectangle.width)
                   + " height: " + std::to_string(rectangle.height) + "}";
        },
        sol::meta_function::equal_to, &math::Rectangle::operator==);

    rec["x"] = &math::Rectangle::x;
    rec["y"] = &math::Rectangle::y;
    rec["width"] = &math::Rectangle::width;
    rec["height"] = &math::Rectangle::height;
}

void LuaManager::InitLuaEventHandler()
{
    instance_->luaEventManager_
        = LuaCallable(instance_->lua_, "EventHandler", "handleEvents", scripts::EventHandlerScript);

    instance_->luaEventManager_.registerAsGlobal(instance_->lua_, "gEventHandler");
}

void LuaManager::InitECS()
{
    auto entityManager = instance_->lua_.new_usertype<ecs::EntityManager>("EntityManager");
    entityManager.set_function("createEntity", &ecs::EntityManager::createEntity);

    auto entity = instance_->lua_.new_usertype<ecs::Entity>(
        "Entity", sol::constructors<sol::types<entt::entity, entt::registry&>>());

    entity.set_function("addChild", &ecs::Entity::addChild);
    entity.set_function("setParent", &ecs::Entity::setParent);

    auto graphicsComponent = instance_->lua_.new_usertype<ecs::components::GraphicsComponent>(
        "GraphicsComponent",
        sol::constructors<sol::types<std::string_view, math::Rectangle>,
                          sol::types<std::string_view, math::Rectangle, std::uint8_t>>());

    graphicsComponent["srcRectangle"] = &ecs::components::GraphicsComponent::srcRectangle;
    graphicsComponent["guid"] = &ecs::components::GraphicsComponent::guid;

    REGISTER_COMPONENT_WITH_ECS(
        instance_->lua_, GraphicsComponent,
        [](ecs::Entity& self, const std::string& str, const math::Rectangle& rect)
        { return std::ref(self.addComponent<ecs::components::GraphicsComponent>(str, rect)); },
        [](ecs::Entity& self, const std::string& str, const math::Rectangle& rect)
        { return std::ref(self.addComponent<ecs::components::GraphicsComponent>(str, rect)); });

    auto transformComponent = instance_->lua_.new_usertype<ecs::components::TransformComponent>(
        "TransformComponent",
        sol::constructors<sol::types<math::Vector2, math::Vector2, float, std::uint8_t>>());

    transformComponent["transform"] = &ecs::components::TransformComponent::position;
    transformComponent["scale"] = &ecs::components::TransformComponent::scale;
    transformComponent["rotation"] = &ecs::components::TransformComponent::rotation;
    transformComponent["zLayer"] = &ecs::components::TransformComponent::zLayer;

    REGISTER_COMPONENT_WITH_ECS(
        instance_->lua_, TransformComponent, [](ecs::Entity& self, const math::Vector2& vec)
        { return std::ref(self.addComponent<ecs::components::TransformComponent>(vec)); });

    auto factionComponent = instance_->lua_.new_usertype<ecs::components::FactionComponent>(
        "FactionComponent", sol::constructors<sol::types<std::bitset<8>>>());

    factionComponent["factionMask"] = &ecs::components::FactionComponent::factionMask;

    REGISTER_COMPONENT_WITH_ECS(
        instance_->lua_, FactionComponent, [](ecs::Entity& self, const std::uint8_t& mask)
        { return std::ref(self.addComponent<ecs::components::FactionComponent>(mask)); });
}

template <typename EventType, typename... Args>
std::function<void(events::EventQueue&, Args...)> make_enqueue_lambda()
{
    return [](events::EventQueue& q, Args... args)
    { q.enqueue<EventType>(std::forward<Args>(args)...); };
}

#define REGISTER_EVENT_TYPE(curLuaState, Ev, ...)                                           \
    {                                                                                       \
        auto event_type                                                                     \
            = curLuaState["EventQueue"].get_or_create<sol::usertype<events::EventQueue>>(); \
        event_type.set_function(CONCAT(enqueue, Ev),                                        \
                                make_enqueue_lambda<events::Ev, __VA_ARGS__>());            \
    }

void LuaManager::InitScene()
{
    instance_->lua_.set("scene", instance_->scene_);

    auto scene = instance_->lua_.new_usertype<interfaces::Scene>("Scene");
    scene.set_function("getEntityManager", &interfaces::Scene::getEntityManager);
    scene.set_function("getEventQueue", &interfaces::Scene::getEventQueue);
    scene.set_function("getApplication", &interfaces::Scene::getApplication);

    auto eventQueue = instance_->lua_.new_usertype<events::EventQueue>("EventQueue");
    REGISTER_EVENT_TYPE(instance_->lua_, MouseEvent, math::Vector2, std::int32_t)
    REGISTER_EVENT_TYPE(instance_->lua_, KeyEvent, KeyCodes::Key)
}

void LuaManager::InitUtils()
{
    auto scene = instance_->lua_.set_function("hashString", &hashString);
}

void LuaManager::InitEventTypes()
{
    // clang-format off

    instance_->lua_.new_usertype<events::Event>("Event",
        sol::no_constructor,
        "getEventType", &events::Event::getEventType,
        "getName", &events::Event::getName,
        "ToString", &events::Event::ToString);

    instance_->lua_.new_usertype<events::MouseEvent>("MouseEvent",
        sol::constructors<events::MouseEvent(math::Vector2, std::int32_t)>(),
        sol::base_classes, sol::bases<events::Event>(),
        "position", &events::MouseEvent::position,
        "mouseButton", &events::MouseEvent::mouseButton);

    instance_->lua_.new_usertype<events::KeyEvent>("KeyEvent",
        sol::constructors<events::KeyEvent(KeyCodes::Key)>(),
        sol::base_classes, sol::bases<events::Event>(),
        "key", &events::KeyEvent::key);

    instance_->lua_.set_function("as_mouse_event", [](const std::shared_ptr<events::Event>& event)
                                 { return std::dynamic_pointer_cast<events::MouseEvent>(event); });

    instance_->lua_.set_function("as_keyboard_event", [](const std::shared_ptr<events::Event>& event)
                                 { return std::dynamic_pointer_cast<events::KeyEvent>(event); });

    // clang-format on
}

void LuaManager::update()
{
    for (auto& update : updateables_)
    {
        update();
    }
}

}  // namespace roen::lua
