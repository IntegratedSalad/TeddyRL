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
    
    std::map<sf::Keyboard::Key, Action> bindings = in_game_bindings;
    
    while (Engine::getEngineIsRunning() == EngineState::STATE_RUNNING)
    {
        sf::Event event;
        Action player_action = Action::ACTION_IDLE;
        while (window->pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    delete playerTile;
                    window->close();
                    return EngineState::STATE_EXITING;
                }
                case sf::Event::KeyPressed:
                {
                    player_action = returnActionFromInput(bindings, event.key.code);
//                    std::cout << static_cast<int>(player_action) << std::endl;
                }
                default:
                {
                }
            }
        }

        window->clear();
        
        if (player_action == Action::ACTION_MOVE_E)
        {
            player->tile->move(C_TILE_SIZE, 0);
        }

        /* DRAW */
        
//        this->renderAll();
        
        window->draw(*(player->tile)); // tile should be a reference, not a pointer
        window->display();
    }
    

}

void Engine::renderAll()
{
    /* Render Game Map */
    
    
    /* Render Windows */
    
    
}
