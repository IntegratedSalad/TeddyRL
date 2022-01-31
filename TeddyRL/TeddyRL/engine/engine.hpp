#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#ifdef __APPLE__
#include "../ResourcePath.hpp"
#endif

#include "entity.hpp"
#include "map.hpp"
#include "constants.hpp"


/* TODO: all caps */
enum class EngineState
{
    STATE_RUNNING = 0,
    STATE_SAVING,
    STATE_EXITING,
    STATE_MENU
    
};

/* TODO: Think about making this an abstract class. */
class Engine
{
private:
    EngineState isRunning;
    Entity* player;
    Map* gameMap;

public:
    Engine();
    EngineState mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> tilesetVector);
    
    EngineState getEngineIsRunning(void)
    {
        return this->isRunning;
    }

    void renderAll();
    
    /* Scale map coordinates to screen coordinates */
    int getCordObjToDraw(int i)
    {
        return i * C_TILE_SIZE;
    }
};

