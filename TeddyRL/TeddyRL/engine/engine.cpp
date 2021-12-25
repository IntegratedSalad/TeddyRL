#include "engine.hpp"
#include "entity.hpp"

Engine::Engine()
{
    isRunning = EngineState::STATE_RUNNING;
}

EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> spritesVector)
{
    sf::Font font;
    
    Tile playerTile = Tile{};
    
    player = new Entity{};
    
    if (!font.loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        
        return EngineState::STATE_EXITING;

    }

    while (Engine::getEngineIsRunning() == EngineState::STATE_RUNNING)
    {

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        window->clear();

        /* DRAW */
        
        

        window->display();
    }
    

}

void Engine::renderAll()
{
    /* Render Game Map */
    
    
    /* Render Windows */
    
    
}
