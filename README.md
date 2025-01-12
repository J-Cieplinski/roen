# Roen

Companing project to [**spielda**](https://github.com/J-Cieplinski/spielda) created alongside it for the purpose of abstracting cross-project code to create a personal 2D game development framework

## Features

* Class-based system container for storing all of the systems object using their typeid as a key 
* Implementation of A* pathfinding algorithm
* Templated assets manager
* Game scene manager with deferred scene switching
* Logging system
* Loader of [**Tiled**](http://www.mapeditor.org/) maps with specific Layer types and properties

## Planned and in development
* Further abstraction of code from main project

## Tech stack

- C++23
- STL

### Dependencies
- [**entt**](https://github.com/skypjack/entt): header-only, tiny and easy to use library for game programming and much more written in modern C++.
- [**raylib**](https://github.com/raysan5/raylib): raylib is a simple and easy-to-use library to enjoy videogames programming.
- [**nlohmann_json**](https://github.com/nlohmann/json): json library
- [**tileson**](https://github.com/SSBMTonberry/tileson): json-parser for C++, used for parsing Tiled maps
- [**tileson**](https://github.com/gabime/spdlog/): Fast C++ logging library
- [**GoogleTest**](https://github.com/google/googletest): Google's C++ test framework.

## Usage

Add the library as a dependency to your project, update its submodules and include its CMakeList

#### Disclaimer

This is all a work in progress, some things may not work as expected