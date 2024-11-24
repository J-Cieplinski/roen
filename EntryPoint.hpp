#include <iostream>

extern std::unique_ptr<roen::Application> roen::createApplication();

int main()
{
    //sol::state lua;
    std::string str {"Print from lua func"};
    //lua.set_function("print", [&str]{ APP_INFO("{0}", str); });

    std::cout << "Hello, World!" << std::endl;
    auto application = roen::createApplication();
    application->init();
    application->run();
    //lua.script("print()");

    return 0;
}
