target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/LuaCallable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LuaEntityHandle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LuaManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LuaScript.cpp
    PUBLIC
    ${INCLUDE_DIR}/lua/LuaCallable.hpp
    ${INCLUDE_DIR}/lua/LuaEntityHandle.hpp
    ${INCLUDE_DIR}/lua/LuaManager.hpp
    ${INCLUDE_DIR}/lua/LuaScript.hpp
    ${INCLUDE_DIR}/lua/scripts/LuaEventHandler.hpp
)