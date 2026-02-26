#include <iostream>

extern std::unique_ptr<roen::Application> roen::createApplication();

int main()
{
    auto application = roen::createApplication();
    application->init();
    application->run();

    return 0;
}
