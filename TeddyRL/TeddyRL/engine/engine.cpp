#include "engine.hpp"
#include "entity.hpp"
#include "constants.hpp"
#include "handleKeys.hpp"
#include "map.hpp"
#include "utils.hpp"

#include "drawing_utils.hpp"
#include <random>
#include <algorithm>

static bool debugModeOn = false;
static bool isShiftPressed = false; // only because on macOS whitelisting input monitoring doesn't work...
static bool cameraMoved = false;
Engine::Engine()
{
    engineState = EngineState::STATE_RUNNING;
    sf::Font* _font = new sf::Font;
    
    if (!_font->loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        return;

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
    
    Entity* camerap = new Entity{};
    camerap->SetX(player->GetX());
    camerap->SetY(player->GetY());
    camerap->SetTile(nullptr);
    camerap->SetName("camera");
    
    bool mouseActivated = false;
    GameState turn = GameState::PLAYER_AND_FRIENDS_TURN;

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
                    if (event.key.shift)
                    {
                        isShiftPressed = true;
                    } else
                    {
                        cameraMoved = false;
                    }
                    playerAction = ReturnActionFromInput(bindings, event.key.code);
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
            turn = HandlePlayerAction(player, playerAction, gameMap->blockingEntitiesInt2DVector, gameMap->blockingEntities, playerActionResult, camerap); // turn results are written in function, in a variable passed by reference.
            
            // Player starts first!
                
            if (playerActionResult.type == ActionType::ACTIONTYPE_ATTACK)
            {
                // always the player
                Entity* attackerEntityPointer =  gameMap->GetBlockingEntityPointerFromEntityVectorPos(playerActionResult.entityPerformingActionVectorPos);
                Entity* targetEntityPointer = gameMap->GetBlockingEntityPointerFromEntityVectorPos(playerActionResult.entityTargetOfActionVectorPos);
                
                std::cout << attackerEntityPointer->GetName() + " attacks " << targetEntityPointer->GetName() << std::endl;
                
                /* Killing entities. */
                
                /* Free space in the blockingEntitiesInt2DVector */
                /* Point the dead entity in blockingEntities to nullptr */
                gameMap->RemoveEntityFromMap(targetEntityPointer);
                /* Free memory of the entity and its components */
                gameMap->KillEntity(targetEntityPointer);
                std::cout << targetEntityPointer->GetName() << " dies!" << std::endl;
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
                Actor* actorp = nullptr;
                if (ep == nullptr) continue;
                actorp = ep->GetActorComponent();
                
                /* AI makes a turn - returns turn result that should be applied.
                   If it moves - it applies changes to the Entity object.
                   If it wants to heal itself, drink something, or do anything related to Actor, it returns turn result
                   tied to an Actor component.
                   
                 */
                if (actorp != nullptr) // Somehow the actorp still points to something
                {
                    /* AI makes turn */
                    AI* aip = actorp->GetAI();
                    if (aip == nullptr) continue;
                    aiActionResult = aip->MakeTurn(*gameMap, player, rng);
                    if (aiActionResult.type == ActionType::ACTIONTYPE_MOVE)
                    {
                        // Move.
                        
                        // use switch here
                        Direction dirData = static_cast<Direction>(aiActionResult.m_ActionData["direction"]);
                        std::tuple<int, int> directions = MapDirectionToCoordinates(dirData);
                        int x = std::get<0>(directions);
                        int y = std::get<1>(directions);
                        ActionResult moveActionResult = ep->MoveOrBump(x, y, gameMap->blockingEntitiesInt2DVector, gameMap->blockingEntities);
                        
                        if (moveActionResult.type == ActionType::ACTIONTYPE_ATTACK)
                        {
                            // get Actor data and make attack
                            int performerVecPos = moveActionResult.entityPerformingActionVectorPos; // always current ep
                            int targetVecPos = moveActionResult.entityTargetOfActionVectorPos;
                            
                            Entity* performerEntityPointer = gameMap->GetBlockingEntityPointerFromEntityVectorPos(performerVecPos);
                            Entity* targetEntityPointer = gameMap->GetBlockingEntityPointerFromEntityVectorPos(targetVecPos);
                            
                            std::cout << performerEntityPointer->GetName() << " attacks: " << targetEntityPointer->GetName() << std::endl;
                            /* Killing entities. */
                            
                            /* Free space in the blockingEntitiesInt2DVector */
                            /* Point the dead entity in blockingEntities to nullptr */
                            gameMap->RemoveEntityFromMap(targetEntityPointer);
                            /* Free memory of the entity and its components */
                            gameMap->KillEntity(targetEntityPointer);
                            std::cout << targetEntityPointer->GetName() << " dies!" << std::endl;
                            
                            if (targetEntityPointer == player)
                                this->setEngineState(EngineState::STATE_GAME_OVER);
                        }
                    }
                    // TODO: What if we'll design time system, and monster will deplete its energy, making player have two actions?
                }
            }
            turn = GameState::PLAYER_AND_FRIENDS_TURN;
        }
        
        /* DRAW */
        
        this->RenderAll(gameMap->blockingEntitiesInt2DVector, gameMap->blockingEntities, window, *gameMap, camerap);

        currentTime = clock.getElapsedTime();
        fps = 1.0f / previousTime.asSeconds() - currentTime.asSeconds();
        std::string fpsString = std::to_string(floor(fps));
        
        if (debugModeOn)
        {
            DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, fpsString, 0, (C_MAP_SIZE - 1) * C_TILE_IN_GAME_SIZE, *gameFont);
        }
        window->display();
        
        if (saveGame) // Don't allow for save scumming exiting results in a save and you can only load from main menu
        {
            // also, saving exits the game <- exits to the main menu
            delete camerap;
            return EngineState::STATE_SAVING;
        }

        if (!isShiftPressed && !cameraMoved)
        {
            camerap->SetX(player->GetX());
            camerap->SetY(player->GetY());
        }
        isShiftPressed = false;
    }
    
    if (this->engineState == EngineState::STATE_GAME_OVER)
    {
        this->RenderGameOver(window);
        std::cout << "dupa" << std::endl;
        delete camerap;
        return EngineState::STATE_GAME_OVER;
    }
    return EngineState::STATE_MENU; // for now, assume we are going to the menu.
}
#warning entityVector should be a const reference.
#warning remember about vertex array.
void Engine::RenderAll(Int2DVec intVec, std::vector<Entity* >& blockingEntities, sf::RenderWindow* window, const Map& map, const Entity* cameraPointer) const
{
    /* Render Game Map */
    
    // TODO: Monsters have to have black background or background that means something (status or etc.)
    
    sf::View gameView;
    gameView.reset(sf::FloatRect( (cameraPointer->GetX() - (C_CAMERA_RANGE  / 2)) * C_TILE_IN_GAME_SIZE,
                                  (cameraPointer->GetY() - (C_CAMERA_RANGE  / 2)) * C_TILE_IN_GAME_SIZE,
                                  C_CAMERA_RANGE * C_TILE_IN_GAME_SIZE,
                                  C_CAMERA_RANGE * C_TILE_IN_GAME_SIZE)); // + offsets
    
    //gameView.setViewport(sf::FloatRect(0.135f, 0.15f, 0.65f, 0.65f)); // maybe define
    gameView.setViewport(sf::FloatRect(0.f, 0.f, 0.65f, 1.f));
    window->setView(gameView);
    
    // TODO: Zooming out and in.
    
    std::vector<Tile*> blockingEntitiesInCameraRange = GetSpritesOfEntitiesCloseToPlayer(blockingEntities, cameraPointer);
    
    for (auto& t : blockingEntitiesInCameraRange)
    {
        window->draw(*t);
    }
    
    blockingEntitiesInCameraRange.clear();
    
    if (debugModeOn)
        RenderDebugInfo(map, this->player, window);
    
    /* Render Panels */
    /* Panel will be a section of the Window e.g. GUI */
    
}

GameState Engine::HandlePlayerAction(Entity* player, PlayerAction playerAction, Int2DVec& intVec, std::vector<Entity* > actorsVector, ActionResult& turnAction, Entity* camera)
{
    switch (playerAction)
    {
        case PlayerAction::PLR_ACTION_MOVE_N:
            if (isShiftPressed)
            {
                camera->SetY(camera->GetY() - C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(0, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_NE:
            if (isShiftPressed)
            {
                camera->SetX(camera->GetX() + C_CAMERA_SHIFT_OFFSET);
                camera->SetY(camera->GetY() - C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(1, -1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
        
        case PlayerAction::PLR_ACTION_MOVE_E:
            if (isShiftPressed)
            {
                camera->SetX(camera->GetX() + C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(1, 0, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_SE:
            if (isShiftPressed)
            {
                camera->SetX(camera->GetX() + C_CAMERA_SHIFT_OFFSET);
                camera->SetY(camera->GetY() + C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(1, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_S:
            if (isShiftPressed)
            {
                camera->SetY(camera->GetY() + C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(0, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_SW:
            if (isShiftPressed)
            {
                camera->SetX(camera->GetX() - C_CAMERA_SHIFT_OFFSET);
                camera->SetY(camera->GetY() + C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(-1, 1, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_W:
            if (isShiftPressed)
            {
                camera->SetX(camera->GetX() - C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(-1, 0, intVec, actorsVector);
            return GameState::ENEMY_TURN;
            
        case PlayerAction::PLR_ACTION_MOVE_NW:
            if (isShiftPressed)
            {
                camera->SetX(camera->GetX() - C_CAMERA_SHIFT_OFFSET);
                camera->SetY(camera->GetY() - C_CAMERA_SHIFT_OFFSET);
                cameraMoved = true;
                return GameState::PLAYER_AND_FRIENDS_TURN;
            }
            turnAction = player->MoveOrBump(-1, -1, intVec, actorsVector);
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

void Engine::RenderDebugInfo(const Map& map, const Entity* player, sf::RenderWindow* window) const
{
    /* TODO: Spawn a rectangle that will be a "cursor". This cursor then will directly point to
             objects on screen.
     */
    
    //DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, "DEBUG", 0, -8, *this->gameFont);

    /* TODO: Draw a grey rectangle on the bottom left corner with debug information */
    
    int mouseXPositionRelative = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x; // TODO: Const
    int mouseYPositionRelative = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y; // TODO: Const

    // TODO: Calculate the distance between nearest blocking entity and show it, not rely on anything in map logic
    
    if ((mouseXPositionRelative && mouseYPositionRelative >= 0))
    {
        int mouseXPositionScaled = (int)floor(mouseXPositionRelative / C_TILE_IN_GAME_SIZE);
        int mouseYPositionScaled = (int)floor(mouseYPositionRelative/ C_TILE_IN_GAME_SIZE);
        
        if (mouseXPositionScaled < C_MAP_SIZE && mouseYPositionScaled < C_MAP_SIZE)
        {
            int debugTextPosX = mouseXPositionRelative;
            int debugTextPosY = mouseYPositionRelative;
            
            Entity* ep = map.GetBlockingEntityPointerFromLocation(mouseXPositionScaled, mouseYPositionScaled);
            
            if (ep != nullptr)
            {
                DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, std::to_string(map.GetBlockingEntityIndexFromLocation(mouseXPositionScaled, mouseYPositionScaled)), debugTextPosX, debugTextPosY + 32, *gameFont);
                
                debugTextPosY += 64;
                const std::string positionText = "x: " + std::to_string(ep->GetX()) + " y: " + std::to_string(ep->GetY());
                
                DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, positionText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                const std::string nameText = "Name: " + ep->GetName();
                
                DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, nameText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                const std::string canBlockText = "Can block :" + std::to_string(ep->tile->canBlock);
                
                DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, canBlockText, debugTextPosX, debugTextPosY, *gameFont);
                
                debugTextPosY += 32;
                
                const std::string actorComponentText = "Actor component: " + std::to_string(!(ep->GetActorComponent() == nullptr));
                DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 16, actorComponentText, debugTextPosX, debugTextPosY, *gameFont);
            }
        }
    }
    window->setView(window->getDefaultView());
    DrawTextOnRectangle(window, sf::Color::Black, sf::Color::White, 32, "DEBUG", 0, -8, *this->gameFont);
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
    return EngineState::STATE_EXITING;
}

void Engine::SetupNewGameMap(const std::vector<sf::Sprite> spritesVector)
{
#warning Important!
    Map* mp = new Map(spritesVector);
    mp->SetupLevelInformation();
        
    // Make Level
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
    mp->GenerateLevel(); // this is only a method to intialize a new game, not for loading the map!
    
    Entity* player = new Entity{
            spritesVector,
            73,
            false,
            true,
            sf::Color::White,
            TileSprite::TEDDY,
            true,
            AIType::NONE,
            "Teddy",
            0,
            0
    };
    
    mp->PlaceBlockingEntityInRandomPlaceInRoom(player, mp->GetStartingRoom(), rng);
    
    this->player = player;

    std::cout << mp->blockingEntities.size() << std::endl;
    this->gameMap = mp;
}

void Engine::LoadGameMap(const std::vector<sf::Sprite> spritesVector, Map* mp)
{
    // TODO: Move setting up from app.cpp here.
    this->gameMap = mp;
}

std::vector<Tile*> Engine::GetSpritesOfEntitiesCloseToPlayer(const std::vector<Entity*>& entities, const Entity *cameraPointer) const
{
    std::vector<Tile*> vt;
    
    for (const Entity* bep: entities)
    {
        if (bep != nullptr)
        {
            if (DistanceBetweenTwoEntities(*bep, *cameraPointer) <= C_CAMERA_RANGE)
            {
                vt.push_back(bep->GetTile());
            }
        }
    }
    return vt;
}

void Engine::ClearGameMap(void)
{
    this->gameMap->Clear();
}
