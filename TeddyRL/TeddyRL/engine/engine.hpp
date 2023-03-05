#include <SFML/Graphics.hpp>
#include <iostream>
#ifdef __APPLE__
#include "../ResourcePath.hpp"
#endif

#include "entity.hpp"
#include "map.hpp" // instead of including map, maybe just type forward declaration?
#include "constants.hpp"

#include "PlayerActions.h"
#include "TurnActions.hpp"
#include "tile.hpp"

/* TODO: all caps */
enum class EngineState
{
    STATE_RUNNING = 0,
    STATE_SAVING,
    STATE_EXITING,
    STATE_MENU,
    STATE_GAME_OVER
};

enum class GameState
{
    PLAYER_AND_FRIENDS_TURN = 0,
    ENEMY_TURN,
};

//typedef std::map<std::string, std::tuple<std::string, std::string, std::string>> ActionResultMap;

class Engine
{
private:
    EngineState engineState;
    Entity* player;
    Map* gameMap;
    
    bool saveGame = false;
    
    void PrepareToExit(void);
    
    //std::string saveDirPath;

public:
    sf::Font* gameFont;
    
    Engine();
    ~Engine();
    EngineState mainLoop(sf::RenderWindow* window, std::mt19937& rng);
    
//    void setDirPath(const std::string& p)
//    {
//        saveDirPath = p;
//    }
    
    EngineState getEngineState(void)
    {
        return this->engineState;
    }
    
    void setEngineState(EngineState s)
    {
        this->engineState = s;
    }
    
    const Map* GetGameMap(void) const
    {
        return this->gameMap;
    }
    
    void SetPlayer(Entity* e) {this->player = e;}

    void RenderAll(Int2DVec, std::vector<Entity* > entityVector, sf::RenderWindow* window, const Map&) const;
    EngineState RenderGameOver(sf::RenderWindow* window) const;
    
    /* Scale map coordinates to screen coordinates */
//    int getCordObjToDraw(int i)
//    {
//        return i * C_TILE_SIZE;
//    }
//    
    
    GameState HandlePlayerAction(Entity* player, PlayerAction playerAction, Int2DVec&, std::vector<Entity* > entityVector, ActionResult&);
    void RenderDebugInfo(const Map&, const Entity* player, sf::RenderWindow* window) const;
    
    void SetupNewGameMap(const std::vector<sf::Sprite> spritesVector);
    void LoadGameMap(const std::vector<sf::Sprite> spritesVector, Map* mp);
    
};

class TurnExecutor
{
    
};
