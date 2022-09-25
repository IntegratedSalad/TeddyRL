#include "engine.hpp"
#include "entity.hpp"
#include "constants.hpp"
#include "handleKeys.hpp"
#include "map.hpp"
#include "utils.hpp"

#include "drawing_utils.hpp"
#include <random>

#define DEBUG 1

bool debugModeOn = false;

Engine::Engine()
{
    engineState = EngineState::STATE_RUNNING;
    
    sf::Font* _font = new sf::Font;
    
    if (!_font->loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        
        return EngineState::STATE_EXITING;

    }
    this->gameFont = _font;
}

Engine::~Engine()
{
    delete this->gameFont;
}

EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> spritesVector)
{
    
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
    float fps;
    sf::Clock clock;
    sf::Time previousTime;
    sf::Time currentTime;
    
    bool mouseActivated = false;
    
    Map gameMapObj{}; /* TODO: Think about map and its fields being a private field of the Engine class. */

    sf::Sprite playerSprite = spritesVector[73];
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Entity* player = new Entity{playerTile, "Teddy", 4, 4};
    /* player is manually added before every entity, its entityVectorPos is 0. */

    gameMapObj.placeEntityOnMap(player, player->getX(), player->getY());
    this->player = player;
    
    gameMapObj.generateLevel(spritesVector, rng);
    
    std::cout << gameMapObj.entityVector.size() << std::endl;
    
    GameState turn = GameState::PLAYER_AND_FRIENDS_TURN;

    /* Game Map View, Camera system */
    
    /* TODO:
     1. Try to do a gameMapView -> portion of the screen rendered as it would be normally, but scaled and positioned at (16, 16). Make tiles scaled as well.
     */
//
//    std::cout << player->tile->getPosition().x << player->tile->getPosition().y << std::endl;
    
    /*               */
    
    KeyActionMap bindings = inGameBindings;

    while (Engine::getEngineState() == EngineState::STATE_RUNNING)
    {
        sf::Event event;
        previousTime = clock.getElapsedTime();
        clock.restart();

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
        
        /* Player & friends turn */
        
        TurnAction playerTurnResults;
        if (turn == GameState::PLAYER_AND_FRIENDS_TURN)
        {
            turn = handlePlayerAction(player, playerAction, gameMapObj.entityIntVec, gameMapObj.entityVector, playerTurnResults); // turn results are written in function, in a variable passed by reference.
                
            if (playerTurnResults.name == "attack") // placeholder
            {
                Entity* attackerEntityPointer = gameMapObj.getEntityPointerFromEntityVectorPos(playerTurnResults.entityPerformingActionVectorPos);
                Entity* targetEntityPointer = gameMapObj.getEntityPointerFromEntityVectorPos(playerTurnResults.entityTargetOfActionVectorPos);
                
                std::cout << attackerEntityPointer->getName() + "attacks " << targetEntityPointer->getName() << std::endl;
                
            }
            
            if (playerTurnResults.name == "bump") // placeholder
            {
                Entity* bumperEntityPointer = gameMapObj.getEntityPointerFromEntityVectorPos(playerTurnResults.entityPerformingActionVectorPos);
                Entity* targetEntityPointer = gameMapObj.getEntityPointerFromEntityVectorPos(playerTurnResults.entityTargetOfActionVectorPos);
                
                std::cout << bumperEntityPointer->getName() + "bumps into " << targetEntityPointer->getName() << std::endl;
                
            }
        }
        
        // TODO: Execute player turn
        /* Enemies turn */
        
        TurnAction aiTurnResults;
        if (turn == GameState::ENEMY_TURN)
        {
        
            for (int i = 1; i < gameMapObj.entityVector.size(); i++) // use iterator
            {
                Actor* ap = gameMapObj.entityVector[i]->getActorComponent();
                if (ap != nullptr)
                {
                  aiTurnResults = ap->make_turn(gameMapObj.entityIntVec, gameMapObj.entityVector, gameMapObj, rng, player);
                    
                    // TODO: Execute enemy turn
                    // TODO: What if we'll design time system, and monster will deplete its energy, making player have two actions?
                }
            }
            turn = GameState::PLAYER_AND_FRIENDS_TURN;
        }
        
        /* DRAW */
        
        this->renderAll(gameMapObj.entityIntVec, gameMapObj.entityVector, window, gameMapObj);

        currentTime = clock.getElapsedTime();
        fps = 1.0f / previousTime.asSeconds() - currentTime.asSeconds();
        
        std::string fpsString = std::to_string(floor(fps));
        
        if (debugModeOn)
        {
            drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, fpsString, 0, (C_MAP_SIZE - 1) * C_TILE_IN_GAME_SIZE, *gameFont);
        }
        
        window->display();
    }
}
#warning entityVector should be a const reference.
#warning remember about pixel array.
void Engine::renderAll(Int2DVec intVec, std::vector<Entity* > entityVector, sf::RenderWindow* window, const Map& map) const
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
    
    if (debugModeOn)
        renderDebugInfo(map, this->player, window);
    
    /* Render Panels */
    /* Panel will be a section of the Window e.g. GUI */
    
}

/* It should return something */
GameState Engine::handlePlayerAction(Entity* player, Action playerAction, Int2DVec& intVec, std::vector<Entity* > entityVector, TurnAction& turnAction)
{
    switch (playerAction)
    {
        case Action::ACTION_MOVE_N:
            turnAction = player->moveOrPerformAction(0, -1, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_NE:
            turnAction = player->moveOrPerformAction(1, -1, intVec, entityVector);
            return GameState::ENEMY_TURN;
        
        case Action::ACTION_MOVE_E:
            turnAction = player->moveOrPerformAction(1, 0, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_SE:
            turnAction = player->moveOrPerformAction(1, 1, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_S:
            turnAction = player->moveOrPerformAction(0, 1, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_SW:
            turnAction = player->moveOrPerformAction(-1, 1, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_W:
            turnAction = player->moveOrPerformAction(-1, 0, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_NW:
            turnAction = player->moveOrPerformAction(-1, -1, intVec, entityVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_PASS_TURN:
            return GameState::ENEMY_TURN;
        
#if DEBUG
        case static_cast<Action>(404):
            
            if (debugModeOn)
            {
                debugModeOn = false;
                std::cout << "Debug mode off." << std::endl;
            } else
            {
                debugModeOn = true;
                std::cout << "Debug mode on." << std::endl;
            }
            
            return GameState::PLAYER_AND_FRIENDS_TURN;
#endif
        case Action::ACTION_IDLE:
            return GameState::PLAYER_AND_FRIENDS_TURN;
    }
}

void Engine::renderDebugInfo(const Map& map, const Entity* player, sf::RenderWindow* window) const
{
    drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, "DEBUG", 0, -8, *this->gameFont);

    int mouseXPositionRelative = sf::Mouse::getPosition(*window).x;
    int mouseYPositionRelative = sf::Mouse::getPosition(*window).y;
    
    if ((mouseXPositionRelative && mouseYPositionRelative >= 0))
    {
        int mouseXPositionScaled = (int)floor(mouseXPositionRelative / C_TILE_IN_GAME_SIZE);
        int mouseYPositionScaled = (int)floor(mouseYPositionRelative/ C_TILE_IN_GAME_SIZE);
        
        if (mouseXPositionScaled < C_MAP_SIZE && mouseYPositionScaled < C_MAP_SIZE)
        {
            int debugTextPosX = mouseXPositionRelative;
            int debugTextPosY = mouseYPositionRelative;
            
            Entity* ep = map.getEntityPointerFromLocation(mouseXPositionScaled, mouseYPositionScaled);
            
            if (ep != nullptr)
            {
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, std::to_string(map.getEntityIndexFromLocation(mouseXPositionScaled, mouseYPositionScaled)), debugTextPosX, debugTextPosY + 32, *gameFont);
                
                debugTextPosY += 64;
                const std::string positionText = "x: " + std::to_string(ep->getX()) + " y: " + std::to_string(ep->getY());
                
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, positionText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                const std::string nameText = "Name: " + ep->getName();
                
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, nameText, debugTextPosX, debugTextPosY, *gameFont);
                
            }
        }
    }
}
