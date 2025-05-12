#include <lua/LuaManager.hpp>

#include <lua/scripts/LuaEventHandler.hpp>

#include <core/Input.hpp>
#include <core/KeyCodes.hpp>

#include <ecs/Entity.hpp>
#include <ecs/EntityManager.hpp>

#include <ecs/components/DirtyComponent.hpp>
#include <ecs/components/FactionComponent.hpp>
#include <ecs/components/GraphicsComponent.hpp>
#include <ecs/components/TransformComponent.hpp>

#include <events/Event.hpp>
#include <events/KeyEvent.hpp>
#include <events/MouseEvent.hpp>

#include <log/Logger.hpp>

#include <Utils.hpp>

namespace roen::lua
{

#define STR(str) #str
#define CONCAT(left, right) STR(left##right)

#define REGISTER_COMPONENT_WITH_ECS(curLuaState, Comp)                                            \
    {                                                                                             \
        namespace component = ecs::components;                                                    \
        auto entity_type = curLuaState["Entity"].get_or_create<sol::usertype<ecs::Entity>>();     \
        entity_type.set_function(CONCAT(remove, Comp),                                            \
                                 &ecs::Entity::removeComponent<component::Comp>);                 \
        entity_type.set_function(CONCAT(get, Comp), &ecs::Entity::getComponent<component::Comp>); \
        entity_type.set_function(CONCAT(add, Comp), &ecs::Entity::addComponent<component::Comp>); \
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
    }

    return *instance_;
}

void LuaManager::onInit(interfaces::Scene* scene)
{
    scene_ = scene;
    InitLua();
    InitLuaInput();
    InitLuaLog();
    InitEventTypes();
    InitECS();
    InitScene();
    InitRaylibTypes();
    InitLuaEventHandler();
    InitUtils();
}

void LuaManager::InitLua()
{
    instance_->lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
                                   sol::lib::table, sol::lib::os, sol::lib::string);
}

void LuaManager::InitLuaInput()
{
    auto input = instance_->lua_["Input"].get_or_create<sol::table>();

    input.set_function(
        "KeyPressed", [](roen::KeyCodes::Key key) -> bool { return roen::Input::KeyPressed(key); });

    input.set_function("KeyReleased", [](roen::KeyCodes::Key key) -> bool
                       { return roen::Input::KeyReleased(key); });

    input.set_function("KeyDown",
                       [](roen::KeyCodes::Key key) -> bool { return roen::Input::KeyDown(key); });

    input.set_function("GetKey", []() -> roen::KeyCodes::Key { return roen::Input::GetKey(); });

    input.set_function("MouseButtonPressed", [](roen::KeyCodes::MouseButton button) -> bool
                       { return roen::Input::MouseButtonPressed(button); });

    input.set_function("MouseButtonReleased", [](roen::KeyCodes::MouseButton button) -> bool
                       { return roen::Input::MouseButtonReleased(button); });

    input.set_function("MouseButtonDown", [](roen::KeyCodes::MouseButton button) -> bool
                       { return roen::Input::MouseButtonDown(button); });

    input.set_function("MousePosition", []() -> Vector2 { return roen::Input::MousePosition(); });

    std::initializer_list<std::pair<sol::string_view, roen::KeyCodes::Key>> keyItems
        = {{"NONE", roen::KeyCodes::Key::NONE},
           {"APOSTROPHE", roen::KeyCodes::Key::APOSTROPHE},
           {"COMMA", roen::KeyCodes::Key::COMMA},
           {"MINUS", roen::KeyCodes::Key::MINUS},
           {"PERIOD", roen::KeyCodes::Key::PERIOD},
           {"SLASH", roen::KeyCodes::Key::SLASH},
           {"ZERO", roen::KeyCodes::Key::ZERO},
           {"ONE", roen::KeyCodes::Key::ONE},
           {"TWO", roen::KeyCodes::Key::TWO},
           {"THREE", roen::KeyCodes::Key::THREE},
           {"FIVE", roen::KeyCodes::Key::FIVE},
           {"SIX", roen::KeyCodes::Key::SIX},
           {"SEVEN", roen::KeyCodes::Key::SEVEN},
           {"EIGHT", roen::KeyCodes::Key::EIGHT},
           {"NINE", roen::KeyCodes::Key::NINE},
           {"SEMICOLON", roen::KeyCodes::Key::SEMICOLON},
           {"EQUAL", roen::KeyCodes::Key::EQUAL},
           {"A", roen::KeyCodes::Key::A},
           {"B", roen::KeyCodes::Key::B},
           {"C", roen::KeyCodes::Key::C},
           {"D", roen::KeyCodes::Key::D},
           {"E", roen::KeyCodes::Key::E},
           {"F", roen::KeyCodes::Key::F},
           {"G", roen::KeyCodes::Key::G},
           {"H", roen::KeyCodes::Key::H},
           {"I", roen::KeyCodes::Key::I},
           {"J", roen::KeyCodes::Key::J},
           {"K", roen::KeyCodes::Key::K},
           {"L", roen::KeyCodes::Key::L},
           {"M", roen::KeyCodes::Key::M},
           {"N", roen::KeyCodes::Key::N},
           {"O", roen::KeyCodes::Key::O},
           {"P", roen::KeyCodes::Key::P},
           {"R", roen::KeyCodes::Key::R},
           {"S", roen::KeyCodes::Key::S},
           {"T", roen::KeyCodes::Key::T},
           {"U", roen::KeyCodes::Key::U},
           {"V", roen::KeyCodes::Key::V},
           {"W", roen::KeyCodes::Key::W},
           {"X", roen::KeyCodes::Key::X},
           {"Y", roen::KeyCodes::Key::Y},
           {"Z", roen::KeyCodes::Key::Z},
           {"LEFT_BRACKET", roen::KeyCodes::Key::LEFT_BRACKET},
           {"BACKSLASH", roen::KeyCodes::Key::BACKSLASH},
           {"RIGHT_BRACKET", roen::KeyCodes::Key::RIGHT_BRACKET},
           {"GRAVE", roen::KeyCodes::Key::GRAVE},
           {"SPACE", roen::KeyCodes::Key::SPACE},
           {"ESCAPE", roen::KeyCodes::Key::ESCAPE},
           {"ENTER", roen::KeyCodes::Key::ENTER},
           {"TAB", roen::KeyCodes::Key::TAB},
           {"BACKSPACE", roen::KeyCodes::Key::BACKSPACE},
           {"INSERT", roen::KeyCodes::Key::INSERT},
           {"DELETE", roen::KeyCodes::Key::DELETE},
           {"RIGHT", roen::KeyCodes::Key::RIGHT},
           {"LEFT", roen::KeyCodes::Key::LEFT},
           {"DOWN", roen::KeyCodes::Key::DOWN},
           {"UP", roen::KeyCodes::Key::UP},
           {"PAGE_UP", roen::KeyCodes::Key::PAGE_UP},
           {"PAGE_DOWN", roen::KeyCodes::Key::PAGE_DOWN},
           {"HOME", roen::KeyCodes::Key::HOME},
           {"END", roen::KeyCodes::Key::END},
           {"CAPS_LOCK", roen::KeyCodes::Key::CAPS_LOCK},
           {"SCROLL_LOCK", roen::KeyCodes::Key::SCROLL_LOCK},
           {"NUM_LOCK", roen::KeyCodes::Key::NUM_LOCK},
           {"PRINT_SCREEN", roen::KeyCodes::Key::PRINT_SCREEN},
           {"PAUSE", roen::KeyCodes::Key::PAUSE},
           {"F1", roen::KeyCodes::Key::F1},
           {"F2", roen::KeyCodes::Key::F2},
           {"F3", roen::KeyCodes::Key::F3},
           {"F4", roen::KeyCodes::Key::F4},
           {"F5", roen::KeyCodes::Key::F5},
           {"F6", roen::KeyCodes::Key::F6},
           {"F7", roen::KeyCodes::Key::F7},
           {"F8", roen::KeyCodes::Key::F8},
           {"F9", roen::KeyCodes::Key::F9},
           {"F10", roen::KeyCodes::Key::F10},
           {"F11", roen::KeyCodes::Key::F11},
           {"F12", roen::KeyCodes::Key::F12},
           {"LEFT_SHIFT", roen::KeyCodes::Key::LEFT_SHIFT},
           {"LEFT_CONTROL", roen::KeyCodes::Key::LEFT_CONTROL},
           {"LEFT_ALT", roen::KeyCodes::Key::LEFT_ALT},
           {"LEFT_SUPER", roen::KeyCodes::Key::LEFT_SUPER},
           {"RIGHT_SHIFT", roen::KeyCodes::Key::RIGHT_SHIFT},
           {"RIGHT_CONTROL", roen::KeyCodes::Key::RIGHT_CONTROL},
           {"RIGHT_ALT", roen::KeyCodes::Key::RIGHT_ALT},
           {"RIGHT_SUPER", roen::KeyCodes::Key::RIGHT_SUPER},
           {"KB_MENU", roen::KeyCodes::Key::KB_MENU},
           {"KP_0", roen::KeyCodes::Key::KP_0},
           {"KP_1", roen::KeyCodes::Key::KP_1},
           {"KP_2", roen::KeyCodes::Key::KP_2},
           {"KP_3", roen::KeyCodes::Key::KP_3},
           {"KP_4", roen::KeyCodes::Key::KP_4},
           {"KP_5", roen::KeyCodes::Key::KP_5},
           {"KP_6", roen::KeyCodes::Key::KP_6},
           {"KP_7", roen::KeyCodes::Key::KP_7},
           {"KP_8", roen::KeyCodes::Key::KP_8},
           {"KP_9", roen::KeyCodes::Key::KP_9},
           {"KP_DECIMAL", roen::KeyCodes::Key::KP_DECIMAL},
           {"KP_DIVIDE", roen::KeyCodes::Key::KP_DIVIDE},
           {"KP_MULTIPLY", roen::KeyCodes::Key::KP_MULTIPLY},
           {"KP_SUBTRACT", roen::KeyCodes::Key::KP_SUBTRACT},
           {"KP_ADD", roen::KeyCodes::Key::KP_ADD},
           {"KP_ENTER", roen::KeyCodes::Key::KP_ENTER},
           {"KP_EQUAL", roen::KeyCodes::Key::KP_EQUAL},
           {"BACK", roen::KeyCodes::Key::BACK},
           {"MENU", roen::KeyCodes::Key::MENU},
           {"VOLUME_UP", roen::KeyCodes::Key::VOLUME_UP},
           {"VOLUME_DOWN", roen::KeyCodes::Key::VOLUME_DOWN}};

    instance_->lua_.new_enum<roen::KeyCodes::Key, false>("Key", keyItems);

    std::initializer_list<std::pair<sol::string_view, roen::KeyCodes::MouseButton>> mouseItems = {
        {"LEFT", roen::KeyCodes::MouseButton::LEFT},
        {"RIGHT", roen::KeyCodes::MouseButton::RIGHT},
        {"MIDDLE", roen::KeyCodes::MouseButton::MIDDLE},
        {"FORWARD", roen::KeyCodes::MouseButton::FORWARD},
        {"BACK", roen::KeyCodes::MouseButton::BACK},
        {"EXTRA", roen::KeyCodes::MouseButton::EXTRA},
        {"SIDE", roen::KeyCodes::MouseButton::SIDE},
    };

    instance_->lua_.new_enum<roen::KeyCodes::MouseButton, false>("MouseButton", mouseItems);
}

#ifdef IS_DEBUG
#    define SET_LOGGER_FUNC(level, name)                                      \
        input.set_function(#name,                                             \
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
        input.set_function(#name, [](sol::variadic_args va) { level(va); });
#endif  // IS_DEBUG

void LuaManager::InitLuaLog()
{
    auto input = instance_->lua_["Logger"].get_or_create<sol::table>();
    auto& state = instance_->lua_;

    SET_LOGGER_FUNC(LUA_ERROR, Error)
    SET_LOGGER_FUNC(LUA_INFO, Info)
    SET_LOGGER_FUNC(LUA_WARN, Warn)
    SET_LOGGER_FUNC(LUA_TRACE, Trace)
    SET_LOGGER_FUNC(LUA_CRITICAL, Critical)
}

void LuaManager::InitRaylibTypes()
{
    auto vec2 = instance_->lua_.new_usertype<Vector2>(
        "Vector2", sol::constructors<Vector2(float, float)>(), sol::meta_function::to_string,
        [](const Vector2& vec) -> std::string
        { return "Vector2 {x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + "}"; });

    vec2["x"] = &Vector2::x;
    vec2["y"] = &Vector2::y;

    auto rec = instance_->lua_.new_usertype<Rectangle>(
        "Rectangle", sol::constructors<Rectangle(float, float, float, float)>(),
        sol::meta_function::to_string,
        [](const Rectangle& rectangle) -> std::string
        {
            return "Rectangle {x: " + std::to_string(rectangle.x) + " y: "
                   + std::to_string(rectangle.y) + " width: " + std::to_string(rectangle.width)
                   + " height: " + std::to_string(rectangle.height) + "}";
        });

    rec["x"] = &Rectangle::x;
    rec["y"] = &Rectangle::y;
    rec["width"] = &Rectangle::width;
    rec["height"] = &Rectangle::height;
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

    REGISTER_COMPONENT_WITH_ECS(instance_->lua_, FactionComponent);

    auto graphicsComponent = instance_->lua_.new_usertype<ecs::components::GraphicsComponent>(
        "GraphicsComponent",
        sol::constructors<sol::types<const std::string&, Rectangle>,
                          sol::types<const std::string&, Rectangle, std::uint8_t>>());

    graphicsComponent["srcRectangle"] = &ecs::components::GraphicsComponent::srcRectangle;
    graphicsComponent["zLayer"] = &ecs::components::GraphicsComponent::zLayer;
    graphicsComponent["guid"] = &ecs::components::GraphicsComponent::guid;

    REGISTER_COMPONENT_WITH_ECS(instance_->lua_, GraphicsComponent);

    auto transformComponent = instance_->lua_.new_usertype<ecs::components::TransformComponent>(
        "TransformComponent", sol::constructors<sol::types<Vector2>>());

    transformComponent["transform"] = &ecs::components::TransformComponent::transform;

    REGISTER_COMPONENT_WITH_ECS(instance_->lua_, TransformComponent);

    auto factionComponent = instance_->lua_.new_usertype<ecs::components::FactionComponent>(
        "FactionComponent", sol::constructors<sol::types<std::bitset<8>>>());

    factionComponent["factionMask"] = &ecs::components::FactionComponent::factionMask;
    REGISTER_COMPONENT_WITH_ECS(instance_->lua_, FactionComponent);
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

    auto eventQueue = instance_->lua_.new_usertype<events::EventQueue>("EventQueue");
    REGISTER_EVENT_TYPE(instance_->lua_, MouseEvent, Vector2, std::int32_t)
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
        sol::constructors<events::MouseEvent(Vector2, std::int32_t)>(),
        sol::base_classes, sol::bases<events::Event>(),
        "position", &events::MouseEvent::position,
        "mouseButton", &events::MouseEvent::mouseButton);

    instance_->lua_.new_usertype<events::KeyEvent>("KeyEvent",
        sol::constructors<events::KeyEvent(roen::KeyCodes::Key)>(),
        sol::base_classes, sol::bases<events::Event>(),
        "key", &events::KeyEvent::key);

    instance_->lua_.set_function("as_mouse_event", [](std::shared_ptr<events::Event> event)
                                 { return std::dynamic_pointer_cast<events::MouseEvent>(event); });

    instance_->lua_.set_function("as_keyboard_event", [](std::shared_ptr<events::Event> event)
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
