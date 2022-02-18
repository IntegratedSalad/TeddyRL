#include "engine.hpp"
#include "entity.hpp"
#include "constants.hpp"
#include "handleKeys.hpp"

Engine::Engine()
{
    isRunning = EngineState::STATE_RUNNING;
}

EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> spritesVector)
{
    
    sf::Sprite playerSprite = spritesVector[0];
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Entity* player = new Entity{playerTile, C_SCREEN_WIDTH / 2, C_SCREEN_HEIGHT / 2};
    
    /* TODO: Move this outside mainLoop()  */
    sf::Font font;
    
    if (!font.loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        
        return EngineState::STATE_EXITING;

    }
    
    /* Game Map View */
    
    /* TODO:
     1. Find a way to nicely draw Tiles (maybe inherit from RectangleShape?) DONE
     2. Try to do a gameMapView -> portion of the screen rendered as it would be normally, but scaled and positioned at (16, 16). Make tiles scaled as well.
     */
//
//    sf::IntRect gameMapViewRect(C_TILE_SIZE, C_TILE_SIZE, C_SCREEN_WIDTH / 2, C_SCREEN_HEIGHT / 2);
//
//
//
//    sf::View gameMapView(sf::FloatRect(C_TILE_SIZE * C_TILE_SIZE, C_TILE_SIZE * C_TILE_SIZE, C_SCREEN_WIDTH / 2, C_SCREEN_HEIGHT / 2));
//

//    sf::Vector2<int> translationVector(-16, -16);
//    gameMapView.reset(sf::FloatRect(translationVector.x, translationVector.y, 16*C_TILE_SIZE, 16*C_TILE_SIZE));

    std::cout << player->tile->getPosition().x << player->tile->getPosition().y << std::endl;

//    window->setView(gameMapView);
//    std::cout << gameMapView.getViewport().height << std::endl;
    
    /*               */
    
    std::map<std::string, Action> bindings = in_game_bindings;
    
    while (Engine::getEngineIsRunning() == EngineState::STATE_RUNNING)
    {
        sf::Event event;
        Action player_action;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                delete playerTile;
                window->close();
                return EngineState::STATE_EXITING;
            } else if (event.type == sf::Event::KeyPressed)
            {
                /* TODO: We have to use event keys, not real-time input. */
                player_action = returnActionFromInput(bindings);
            }
        }
        if (player_action == Action::ACTION_MOVE_S)
        {
            std::cout << "dupa" << std::endl;
        }
        
        window->clear();

        /* DRAW */
        
//        this->renderAll();
        
        window->draw(*(player->tile)); // tile should be a const reference, not a pointer
        window->display();
    }
    

}

void Engine::renderAll()
{
    /* Render Game Map */
    
    
    /* Render Windows */
    
    
}
