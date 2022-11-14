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
    sf::Sprite corpseSprite = spritesVector.at(static_cast<int>(TileSprite::CORPSE));
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Entity* player = new Entity{playerTile, "Teddy", 4, 4};
    /* player is manually added before every entity, its entityVectorPos is 0. */

    gameMapObj.placeBlockingEntityOnMap(player, player->getX(), player->getY());
    this->player = player;
    
    gameMapObj.generateLevel(spritesVector, rng);
    
    std::cout << gameMapObj.blockingEntities.size() << std::endl;
    
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
            turn = handlePlayerAction(player, playerAction, gameMapObj.blockingEntities2DVector, gameMapObj.blockingEntities, playerTurnResults); // turn results are written in function, in a variable passed by reference.
                
            if (playerTurnResults.name == "attack") // placeholder
            {
                Entity* attackerEntityPointer = gameMapObj.getBlockingEntityPointerFromEntityVectorPos(playerTurnResults.entityPerformingActionVectorPos);
                Entity* targetEntityPointer = gameMapObj.getBlockingEntityPointerFromEntityVectorPos(playerTurnResults.entityTargetOfActionVectorPos);
                
                std::cout << attackerEntityPointer->getName() + "attacks " << targetEntityPointer->getName() << std::endl;
                targetEntityPointer->die(corpseSprite);
                gameMapObj.removeEntityFromMap(targetEntityPointer);
            }
        }
        
        // TODO: Execute player turn
        /* Enemies turn */
        
        TurnAction aiTurnResults;
        if (turn == GameState::ENEMY_TURN)
        {
            for (int i = 1; i < gameMapObj.blockingEntities.size(); i++) // use iterator
            {
                Actor* ap = gameMapObj.blockingEntities[i]->getActorComponent();
                if (ap != nullptr)
                {
                    aiTurnResults = ap->make_turn(gameMapObj.blockingEntities2DVector, gameMapObj.blockingEntities, gameMapObj, rng, player);
                    if (aiTurnResults.name == "attack") // placeholder
                    {
                        Entity* attackerEntityPointer = gameMapObj.getBlockingEntityPointerFromEntityVectorPos(aiTurnResults.entityPerformingActionVectorPos);
                        
                        Entity* targetEntityPointer = gameMapObj.getBlockingEntityPointerFromEntityVectorPos(aiTurnResults.entityTargetOfActionVectorPos);
                        
                        std::cout << attackerEntityPointer->getName() + "attacks " << targetEntityPointer->getName() << std::endl;
                        
                        if (targetEntityPointer == player)
                        {
                            this->engineState = EngineState::STATE_GAME_OVER;
                        }
                        // TODO: Player dies.
                    }
                    
                    // TODO: What if we'll design time system, and monster will deplete its energy, making player have two actions?
                }
            }
            turn = GameState::PLAYER_AND_FRIENDS_TURN;
        }
        
        /* DRAW */
        
        this->renderAll(gameMapObj.blockingEntities2DVector, gameMapObj.blockingEntities, window, gameMapObj);

        currentTime = clock.getElapsedTime();
        fps = 1.0f / previousTime.asSeconds() - currentTime.asSeconds();
        
        std::string fpsString = std::to_string(floor(fps));
        
        if (debugModeOn)
        {
            drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, fpsString, 0, (C_MAP_SIZE - 1) * C_TILE_IN_GAME_SIZE, *gameFont);
        }
        
        window->display();
    }
    
    if (this->engineState == EngineState::STATE_GAME_OVER)
    {
        delete playerTile;
        this->RenderGameOver(window);
        std::cout << "dupa" << std::endl;
    }
}
#warning entityVector should be a const reference.
#warning remember about pixel array.
void Engine::renderAll(Int2DVec intVec, std::vector<Entity* > blockingEntities, sf::RenderWindow* window, const Map& map) const
{
    /* Render Game Map */
    
    // TODO: Draw player first, monsters second and items last.
    // TODO: Monsters have to have black background or background that means something (status or etc.)
    
        for (Entity* bep : blockingEntities)
        {
            window->draw(*bep->tile);
        }
    
    
    if (debugModeOn)
        renderDebugInfo(map, this->player, window);
    
    /* Render Panels */
    /* Panel will be a section of the Window e.g. GUI */
    
}

/* It should return something */
GameState Engine::handlePlayerAction(Entity* player, Action playerAction, Int2DVec& intVec, std::vector<Entity* > actorsVector, TurnAction& turnAction)
{
    switch (playerAction)
    {
        case Action::ACTION_MOVE_N:
            turnAction = player->moveOrPerformAction(0, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_NE:
            turnAction = player->moveOrPerformAction(1, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
        
        case Action::ACTION_MOVE_E:
            turnAction = player->moveOrPerformAction(1, 0, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_SE:
            turnAction = player->moveOrPerformAction(1, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_S:
            turnAction = player->moveOrPerformAction(0, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_SW:
            turnAction = player->moveOrPerformAction(-1, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_W:
            turnAction = player->moveOrPerformAction(-1, 0, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case Action::ACTION_MOVE_NW:
            turnAction = player->moveOrPerformAction(-1, -1, intVec, actorsVector);
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
            
            Entity* ep = map.getBlockingEntityPointerFromLocation(mouseXPositionScaled, mouseYPositionScaled);
            
            if (ep != nullptr)
            {
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, std::to_string(map.getBlockingEntityIndexFromLocation(mouseXPositionScaled, mouseYPositionScaled)), debugTextPosX, debugTextPosY + 32, *gameFont);
                
                debugTextPosY += 64;
                const std::string positionText = "x: " + std::to_string(ep->getX()) + " y: " + std::to_string(ep->getY());
                
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, positionText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                const std::string nameText = "Name: " + ep->getName();
                
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, nameText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                const std::string canBlockText = "Can block :" + std::to_string(ep->tile->canBlock);
                
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, canBlockText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                
                const std::string actorComponentText = "Actor component: " + std::to_string(!(ep->getActorComponent() == nullptr));
                drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, actorComponentText, debugTextPosX, debugTextPosY, *gameFont);
            }
        }
    }
}

EngineState Engine::RenderGameOver(sf::RenderWindow* window) const
{
    sf::Event event;
    
    while (this->engineState != EngineState::STATE_EXITING)
    {
        while (window->pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window->close();
                    return EngineState::STATE_EXITING;
                }
                default:
                {
                }
            }
            window->clear();
            DrawText(window, sf::Color::White, 32, "You died.", (C_SCREEN_WIDTH / 2) - 4, (C_SCREEN_HEIGHT / 2), *this->gameFont);
            window->display();
        }
    }
}
