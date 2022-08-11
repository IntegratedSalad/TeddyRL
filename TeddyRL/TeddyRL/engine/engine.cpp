#include "engine.hpp"
#include "entity.hpp"
#include "constants.hpp"
#include "handleKeys.hpp"
#include "map.hpp"
#include "utils.hpp"
#include <random>

Engine::Engine()
{
    engineState = EngineState::STATE_RUNNING;
}

/* IDEA: Maybe something like a Turn Executor?

   Entity wants to move, so we apply the transformation to it + we update the map.
   Every function that is a turn result must return map or a type of some kind, that would hold any information is needed to execute.
 
 */

EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> spritesVector)
{
    
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
    Map gameMapObj{}; /* TODO: Think about map and its fields being a private field of the Engine class. */

    sf::Sprite playerSprite = spritesVector[73];
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Entity* player = new Entity{playerTile, 4, 4};
    /* player is manually added before every entity */

    gameMapObj.placeEntityOnMap(player, player->getX(), player->getY());
    
    gameMapObj.generateLevel(spritesVector, rng);
    
    std::cout << gameMapObj.entityVector.size() << std::endl;
    sf::Font font;
    
    if (!font.loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        
        return EngineState::STATE_EXITING;

    }
    
    /* Game Map View */
    
    /* TODO:
     1. Try to do a gameMapView -> portion of the screen rendered as it would be normally, but scaled and positioned at (16, 16). Make tiles scaled as well.
     */
//
    std::cout << player->tile->getPosition().x << player->tile->getPosition().y << std::endl;
    
    /*               */
    
    KeyActionMap bindings = inGameBindings;

    while (Engine::getEngineState() == EngineState::STATE_RUNNING)
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
//                    debugPrintInt2dVector(gameMapObj.entityIntVec, "map");
                    std::cout << player->getX() << player->getY() << std::endl;
                }
                default:
                {
                }
            }
        }

        window->clear();
        
        /* Player & friends turn */
        
        handlePlayerAction(player, playerAction, gameMapObj.entityIntVec, gameMapObj.entityVector);
        
        
        /* Enemies turn */
        
        for (int i = 1; i < gameMapObj.entityVector.size(); i++) // use iterator
        {
            Actor* ap = gameMapObj.entityVector[i]->getActorComponent();
            if (ap != nullptr)
            {
                ap->make_turn(gameMapObj.entityIntVec, gameMapObj.entityVector, gameMapObj, rng, player);
            }
        }
        


        /* DRAW */
        
        this->renderAll(gameMapObj.entityIntVec, gameMapObj.entityVector, window);
        
        window->display();
    }
    

}
#warning entityVector should be a const reference.
#warning remember about pixel array.
void Engine::renderAll(Int2DVec intVec, std::vector<Entity* > entityVector, sf::RenderWindow* window)
{
    /* Render Game Map */
    
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            if (intVec[i][j] > -1)
            {
                window->draw(*(entityVector[intVec[i][j]]->tile));
            }
        }
    }
    
    /* Render Panels */
    /* Panel will be a section of the Window e.g. GUI */
    
}

/* It should return something */
void Engine::handlePlayerAction(Entity* player, Action playerAction, Int2DVec& intVec, std::vector<Entity* > entityVector)
{
    switch (playerAction)
    {
        case Action::ACTION_MOVE_N:
            player->move(0, -1, intVec, entityVector);
            break;
            
        case Action::ACTION_MOVE_NE:
            player->move(1, -1, intVec, entityVector);
            break;
        
        case Action::ACTION_MOVE_E:
            player->move(1, 0, intVec, entityVector);
            break;
            
        case Action::ACTION_MOVE_SE:
            player->move(1, 1, intVec, entityVector);
            break;
            
        case Action::ACTION_MOVE_S:
            player->move(0, 1, intVec, entityVector);
            break;
            
        case Action::ACTION_MOVE_SW:
            player->move(-1, 1, intVec, entityVector);
            break;
            
        case Action::ACTION_MOVE_W:
            player->move(-1, 0, intVec, entityVector);
            break;
            
        case Action::ACTION_MOVE_NW:
            player->move(-1, -1, intVec, entityVector);
            break;
            
        case Action::ACTION_IDLE:
            break;
    }
}
