#include "app.hpp"

App::App()
{
    auto videoMode = sf::VideoMode(800, 600);
    auto renderWindow = new sf::RenderWindow(videoMode, "Teddy RL");
    window = std::make_unique<sf::RenderWindow>(renderWindow);

    // sf::WindowHandle handle = window->getSystemHandle();ma

    // sf::VideoMode videoMode = sf::VideoMode(800, 600);
    // setWindow(&sf::RenderWindow(videoMode, "TeddyRL"));

    // sf::Window* _window = new sf::Window(videoMode, "Teddy RL");
    // sf::WindowHandle handle = _window->getSystemHandle();
    // sf::RenderWindow renderWindow = sf::RenderWindow(handle);
    // window = std::make_unique<sf::RenderWindow>(sf::RenderWindow::create(handle));


}

App::~App()
{

}

void App::run()
{
   Engine engine = Engine();
   engine.mainLoop(window);

}
