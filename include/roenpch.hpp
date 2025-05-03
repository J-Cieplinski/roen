#ifndef ROEN_ROENPCH_HPP
#define ROEN_ROENPCH_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include <raylib.h>
#include <raymath.h>
#include <spdlog/spdlog.h>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <tileson/tileson.hpp>

#include <Utils.hpp>

#include <algorithms/search/Pathfinding.hpp>
#include <data_structure/Graph.hpp>
#include <data_structure/INode.hpp>
#include <data_structure/MapNode.hpp>
#include <data_structure/PriorityQueue.hpp>
#include <data_structure/SystemsContainer.hpp>
#include <interfaces/IAsset.hpp>
#include <interfaces/ISystem.hpp>
#include <interfaces/Scene.hpp>
#include <loader/BaseMapLoader.hpp>
#include <log/Logger.hpp>
#include <log/formatters/entity.hpp>
#include <manager/AssetManager.hpp>
#include <manager/GameSceneManager.hpp>

#endif  // ROEN_ROENPCH_HPP
