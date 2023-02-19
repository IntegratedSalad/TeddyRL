#include "engine.hpp"
#include "entity.hpp"
#include "constants.hpp"
#include "handleKeys.hpp"
#include "map.hpp"
#include "utils.hpp"

#include "drawing_utils.hpp"
#include <random>

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
    this->gameMap = nullptr;
}

Engine::~Engine()
{
    delete this->gameFont;
    delete this->gameMap;
}

EngineState Engine::mainLoop(sf::RenderWindow* window, std::mt19937& rng)
{
    // pass game map obj, player always initialized before game map obj and placed in it
    /* TODO: More organized initialization - some things don't need to be saved and need to be initialized each time */
    
    float fps;
    sf::Clock clock;
    sf::Time previousTime;
    sf::Time currentTime;
    
    bool mouseActivated = false;
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

        PlayerAction playerAction = PlayerAction::PLR_ACTION_IDLE;
        while (window->pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    /* TODO: Handle Cleanup */
                    /* TODO: Prompt the user to save game. */
                    return EngineState::STATE_EXITING;
                }
                case sf::Event::KeyPressed:
                {
                    playerAction = returnActionFromInput(bindings, event.key.code);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        window->clear();
        
        /* Player & friends turn */
        
        ActionResult playerActionResult;
        if (turn == GameState::PLAYER_AND_FRIENDS_TURN)
        {
            turn = handlePlayerAction(player, playerAction, gameMap->blockingEntitiesInt2DVector, gameMap->blockingEntities, playerActionResult); // turn results are written in function, in a variable passed by reference.
            
            // Player starts first!
                
            if (playerActionResult.type == ActionType::ACTIONTYPE_ATTACK)
            {
                // always the player
                Entity* attackerEntityPointer =  gameMap->getBlockingEntityPointerFromEntityVectorPos(playerActionResult.entityPerformingActionVectorPos);
                Entity* targetEntityPointer = gameMap->getBlockingEntityPointerFromEntityVectorPos(playerActionResult.entityTargetOfActionVectorPos);
                
                std::cout << attackerEntityPointer->getName() + " attacks " << targetEntityPointer->getName() << std::endl;
                
                gameMap->removeEntityFromMap(targetEntityPointer);
                gameMap->KillEntity(targetEntityPointer);
                std::cout << targetEntityPointer->getName() << " dies!" << std::endl;
            }
        }
        /* Friends turn */
        
        /* Enemies turn */
        
        ActionResult aiActionResult;
        if (turn == GameState::ENEMY_TURN)
        {
            for (int i = 1; i < gameMap->blockingEntities.size(); i++)
            {
                Entity* ep = gameMap->blockingEntities[i];
                Actor* actorp = ep->getActorComponent();
                
                /* AI makes a turn - returns turn result that should be applied.
                   If it moves - it applies changes to the Entity object.
                   If it wants to heal itself, drink something, or do anything related to Actor, it returns turn result
                   tied to an Actor component.
                   
                 */
                if (actorp != nullptr)
                {
                    /* AI makes turn */
                    AI* aip = actorp->getAI();
                    aiActionResult = aip->make_turn(*gameMap, player, rng);
                    if (aiActionResult.type == ActionType::ACTIONTYPE_MOVE)
                    {
                        // Move.
                        
                        // use switch here
                        Direction dirData = static_cast<Direction>(aiActionResult.m_ActionData["direction"]);
                        std::tuple<int, int> directions = mapDirectionToCoordinates(dirData);
                        int x = std::get<0>(directions);
                        int y = std::get<1>(directions);
                        ActionResult moveActionResult = ep->moveOrBump(x, y, gameMap->blockingEntitiesInt2DVector, gameMap->blockingEntities);
                        
                        if (moveActionResult.type == ActionType::ACTIONTYPE_ATTACK)
                        {
                            // get Actor data and make attack
                            int performerVecPos = moveActionResult.entityPerformingActionVectorPos; // always current ep
                            int targetVecPos = moveActionResult.entityTargetOfActionVectorPos;
                            
                            Entity* performerEntityPointer = gameMap->getBlockingEntityPointerFromEntityVectorPos(performerVecPos);
                            Entity* targetEntityPointer = gameMap->getBlockingEntityPointerFromEntityVectorPos(targetVecPos);
                            
                            std::cout << performerEntityPointer->getName() << " attacks: " << targetEntityPointer->getName() << std::endl;
                            
                            gameMap->removeEntityFromMap(targetEntityPointer);
                            gameMap->KillEntity(targetEntityPointer);
                            std::cout << targetEntityPointer->getName() << " dies!" << std::endl;
                            this->setEngineState(EngineState::STATE_GAME_OVER);
                        }
                    }
                    // TODO: What if we'll design time system, and monster will deplete its energy, making player have two actions?
                }
            }
            turn = GameState::PLAYER_AND_FRIENDS_TURN;
        }
        
        /* DRAW */
        
        this->renderAll(gameMap->blockingEntitiesInt2DVector, gameMap->blockingEntities, window, *gameMap);

        currentTime = clock.getElapsedTime();
        fps = 1.0f / previousTime.asSeconds() - currentTime.asSeconds();
        std::string fpsString = std::to_string(floor(fps));
        
        if (debugModeOn)
        {
            drawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, fpsString, 0, (C_MAP_SIZE - 1) * C_TILE_IN_GAME_SIZE, *gameFont);
        }
        window->display();
        
        if (saveGame) // Don't allow for save scumming exiting results in a save and you can only load from main menu
        {
            // also, saving exits the game <- exits to the main menu
            // TODO: Saving in app class, not here
            return EngineState::STATE_SAVING;
        }
    }
    
    if (this->engineState == EngineState::STATE_GAME_OVER)
    {
        // destroy save file(s)
        this->RenderGameOver(window);
        std::cout << "dupa" << std::endl;
    }
}
#warning entityVector should be a const reference.
#warning remember about pixel array.
void Engine::renderAll(Int2DVec intVec, std::vector<Entity* > blockingEntities, sf::RenderWindow* window, const Map& map) const
{
    /* Render Game Map */
    
    // TODO: Monsters have to have black background or background that means something (status or etc.)
    
        for (Entity* bep : blockingEntities)
        {
            if (bep != nullptr)
                window->draw(*bep->tile);
        }
    
    if (debugModeOn)
        renderDebugInfo(map, this->player, window);
    
    /* Render Panels */
    /* Panel will be a section of the Window e.g. GUI */
    
}

/* It should return something */
GameState Engine::handlePlayerAction(Entity* player, PlayerAction playerAction, Int2DVec& intVec, std::vector<Entity* > actorsVector, ActionResult& turnAction)
{
    switch (playerAction)
    {
        case PlayerAction::PLR_ACTION_MOVE_N:
            turnAction = player->moveOrBump(0, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_NE:
            turnAction = player->moveOrBump(1, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
        
        case PlayerAction::PLR_ACTION_MOVE_E:
            turnAction = player->moveOrBump(1, 0, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_SE:
            turnAction = player->moveOrBump(1, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_S:
            turnAction = player->moveOrBump(0, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_SW:
            turnAction = player->moveOrBump(-1, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_W:
            turnAction = player->moveOrBump(-1, 0, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_NW:
            turnAction = player->moveOrBump(-1, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_PASS_TURN:
            return GameState::ENEMY_TURN;
        
#if DEBUG
        case static_cast<PlayerAction>(404):
            
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
        case PlayerAction::PLR_ACTION_IDLE:
            turnAction.type = ActionType::ACTIONTYPE_IDLE;
            return GameState::PLAYER_AND_FRIENDS_TURN;
        case PlayerAction::PLR_ACTION_SAVE_GAME:
        {
            // Save Game
            saveGame = true;
            return GameState::PLAYER_AND_FRIENDS_TURN;
        }
            
    }
}

void Engine::renderDebugInfo(const Map& map, const Entity* player, sf::RenderWindow* window) const
{
    /* TODO: Move rendering to another class ?Renderer?
       Maybe renderer will also take care of the camera? e.g. returns a portion of screen to draw.
     
     
     */
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

void Engine::PrepareToExit(void)
{
    /* Free everything */
    
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

void Engine::SetupNewGameMap(const std::vector<sf::Sprite> spritesVector)
{
    Map* mp = new Map(spritesVector);
    
    // Set up player
    
    // Make Level
    
    sf::Sprite playerSprite = spritesVector[73];
    sf::Sprite corpseSprite = spritesVector.at(static_cast<int>(TileSprite::CORPSE)); // this is not an ideal solution
    // maybe create a class that has a vector as a private member, and you can append sprites and get by overloading [] operator,
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    
    Actor* pacp = new Actor{};
    
    Entity* player = new Entity{playerTile, "Teddy", 4, 4};
    player->setActorComponent(pacp);
    /* player is manually added before every entity, its entityVectorPos is 0. */

    mp->placeBlockingEntityOnMap(player, player->getX(), player->getY());
    this->player = player;
    
    mp->generateLevel();
    
    std::cout << mp->blockingEntities.size() << std::endl;
    this->gameMap = mp;
}

void Engine::LoadGameMap(const std::vector<sf::Sprite> spritesVector, Map* mp)
{
    sf::Sprite playerSprite = spritesVector[73];
    sf::Sprite corpseSprite = spritesVector.at(static_cast<int>(TileSprite::CORPSE));
    
    /* Maybe we also have to initialize everything? */
    
    Tile* playerTile = new Tile{false, true, playerSprite, sf::Color::White};
    Actor* pacp = new Actor{};
    Entity* player = new Entity{playerTile, "Teddy", 4, 4};
    player->setActorComponent(pacp);
    mp->placeBlockingEntityOnMap(player, 0, 0);
    
    this->gameMap = mp;
}
