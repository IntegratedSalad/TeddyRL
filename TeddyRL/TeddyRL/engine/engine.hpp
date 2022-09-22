#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#ifdef __APPLE__
#include "../ResourcePath.hpp"
#endif

#include "entity.hpp"
#include "map.hpp"
#include "constants.hpp"

#include "Actions.h"

/* TODO: all caps */
enum class EngineState
{
    STATE_RUNNING = 0,
    STATE_SAVING,
    STATE_EXITING,
    STATE_MENU
    
};

enum class GameState
{
    PLAYER_AND_FRIENDS_TURN = 0,
    ENEMY_TURN,
};

class Engine
{
private:
    EngineState engineState;
    Entity* player;
    Map* gameMap;

public:
    sf::Font* gameFont;
    
    Engine();
    ~Engine();
    EngineState mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> tilesetVector);
    
    EngineState getEngineState(void)
    {
        return this->engineState;
    }

    void renderAll(Int2DVec, std::vector<Entity* > entityVector, sf::RenderWindow* window) const;
    
    /* Scale map coordinates to screen coordinates */
//    int getCordObjToDraw(int i)
//    {
//        return i * C_TILE_SIZE;
//    }
//    
    
    GameState handlePlayerAction(Entity* player, Action playerAction, Int2DVec&, std::vector<Entity* > entityVector);
    
    void renderDebugInfo(const Int2DVec&, const Entity* player, sf::RenderWindow* window) const;
    
    
};

