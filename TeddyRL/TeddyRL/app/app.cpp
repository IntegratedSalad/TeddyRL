#include "app.hpp"

App::App()
{
    sf::VideoMode videoMode = sf::VideoMode(1600, 1200);
    window = new sf::RenderWindow(videoMode, "TeddyRL");
    
    window->setFramerateLimit(60);
    
}

App::~App()
{
    delete window;
}

void App::run()
{
    Tileset tileset("tileset.png", 16);
    
    const std::vector<sf::Sprite> tilesetVector = tileset.getTiles();
    
    Engine engine = Engine();
    EngineState state = engine.mainLoop(window, tilesetVector);

    switch (state)
    {
        case EngineState::STATE_exiting:
            window->close();
            break;
            
        default:
            break;
    }
}
