#include "app.hpp"

App::App()
{
    sf::VideoMode videoMode = sf::VideoMode(800, 800);
    window = new sf::RenderWindow(videoMode, "TeddyRL");

}

App::~App()
{
    delete window;
}

void App::run()
{
   Engine engine = Engine();
   engine.mainLoop(window);

}
