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

/* IDEA: Maybe something like an Turn Executor?

   Entity wants to move, so we apply the transformation to it + we update the map.
   Every function that is a turn result must return map or a type of some kind, that would hold any information is needed to execute.
 
 */

EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> spritesVector)
{
    
    Map gameMapObj{}; /* TODO: Think about map and its fields being a private field of the Engine class. */

    sf::Sprite playerSprite = spritesVector[73];
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Entity* player = new Entity{playerTile, 4, 4};
    /* player is manually added before every entity */
    gameMapObj.entityVector.push_back(player);
    player->entityVectorPos = 0;
    gameMapObj.entityIntArr[player->getX()][player->getY()] = player->entityVectorPos;
    /*                                              */
    
    /* Mock walls */
    
    sf::Sprite wallSprite = spritesVector[128];
    
    Tile* wallTile = new Tile{false, true, wallSprite, sf::Color::White};
    
    Entity* wall = new Entity{wallTile, 0, 0};

    gameMapObj.entityVector.push_back(wall);
    wall->entityVectorPos = 1;
    gameMapObj.entityIntArr[wall->getX()][wall->getY()] = wall->entityVectorPos;
    /*            */
    
    std::cout << gameMapObj.entityVector.size() << std::endl;
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
    
    KeyActionMap bindings = inGameBindings;
    
    while (Engine::getEngineIsRunning() == EngineState::STATE_RUNNING)
    {
        sf::Event event;
        Action playerAction = Action::ACTION_IDLE;
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
                    playerAction = returnActionFromInput(bindings, event.key.code);
                }
                default:
                {
                }
            }
        }

        window->clear();
        
        handlePlayerAction(player, playerAction, gameMapObj.entityIntArr, gameMapObj.entityVector);

        /* DRAW */
        
        this->renderAll(gameMapObj.entityIntArr, gameMapObj.entityVector, window);
        
//        window->draw(*(player->tile)); // tile should be a reference, not a pointer
        window->display();
    }
    

}

void Engine::renderAll(int entityIntArr[C_MAP_SIZE][C_MAP_SIZE], std::vector<Entity* > entityVector, sf::RenderWindow* window)
{
    /* Render Game Map */
    
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            if (entityIntArr[i][j] > -1)
            {
                window->draw(*(entityVector[entityIntArr[i][j]]->tile));
            }
        }
    }
    
    /* Render Windows */
    
    
}

/* It should return something */
void Engine::handlePlayerAction(Entity* player, Action playerAction, int entityIntArr[C_MAP_SIZE][C_MAP_SIZE], std::vector<Entity* > entityVector)
{
    switch (playerAction)
    {
        case Action::ACTION_MOVE_N:
            player->move(0, -1, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_MOVE_NE:
            player->move(1, -1, entityIntArr, entityVector);
            break;
        
        case Action::ACTION_MOVE_E:
            player->move(1, 0, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_MOVE_SE:
            player->move(1, 1, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_MOVE_S:
            player->move(0, 1, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_MOVE_SW:
            player->move(-1, 1, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_MOVE_W:
            player->move(-1, 0, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_MOVE_NW:
            player->move(-1, -1, entityIntArr, entityVector);
            break;
            
        case Action::ACTION_IDLE:
            break;
    }
}
