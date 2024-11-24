#ifndef ROEN_APPLICATION_HPP
#define ROEN_APPLICATION_HPP

#include <manager/GameSceneManager.hpp>

#include <cstdint>
#include <string>

namespace roen
{

class Application
{
public:
    Application(std::uint32_t windowWith, std::uint32_t windowHeight, const std::string& windowTitle = "Roen");
    virtual ~Application();

    virtual void init() = 0;

    void run();
protected:
    manager::GameSceneManager gameSceneManager_;
    bool isRunning_;
};

std::unique_ptr<Application> createApplication();

}

#endif //ROEN_APPLICATION_HPP
