#include "engine.hpp"
#include "entity.hpp"
#include "constants.hpp"
#include "handleKeys.hpp"
#include "map.hpp"

Engine::Engine()
{
    isRunning = EngineState::STATE_RUNNING;
}


/* Maybe declare sprites needed and make them static? we wouldn't need to include spritesVector */
EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> spritesVector)
{
    
    Map game_map{};
    EntityMapVector2D entityVector = game_map.entityMapVector;
    
    /* We need to set tile position of the player to his actual position in game_map */
    /* TODO: We need to design a map system, so that entities are drawn correctly, using only position in our MapVector2D - moving them and positioning them (according to the actual dimensions of the screen) has to be abstracted, so that any manipulation doesn't involve calculating the actual position on the screen. */
    
    sf::Sprite playerSprite = spritesVector[128];
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Entity* player = new Entity{playerTile, 4, 4};
    
    
    
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
    
    KeyActionMap bindings = in_game_bindings;
    
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
            player->move(C_TILE_IN_GAME_SIZE, 0);
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
