#include "app.hpp"
#include "td_serializers.hpp"
#include "constants.hpp"

#if DEBUG && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().parent_path().parent_path().parent_path().string()
#endif

#if RELEASE && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().string()
#endif

static const std::string SAVE_DIR_NAME = "MisioSaves";

App::App(const std::string execPath) : executableDirPath(execPath)
{
    sf::VideoMode videoMode = sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT);
    window = new sf::RenderWindow(videoMode, "TeddyRL");
    
    window->setFramerateLimit(60);
    window->setKeyRepeatEnabled(false);
}

App::~App()
{
    delete this->window;
}

void App::run()
{
    Tileset tileset("tileset.png", C_TILE_TILESET_SIZE);
    
    const std::vector<sf::Sprite> spritesVector = tileset.getSprites();
    
    // Maybe here add class SpriteColection with a getter
    
    Engine engine = Engine();
    //engine.setDirPath(this->executableDirPath);
    const std::string pathToSavedGameFile = ReturnSavedGameFilePath();
    std::filesystem::path execPath = std::filesystem::path(executableDirPath);
    
    /* Load or Setup New Game */
    
    if (0) // if (pathToSavedGameFile.empty())
    {
        CreateSaveGameFolder();
        engine.SetupNewGameMap(spritesVector);
    } else
    {
        std::filesystem::path execPath = std::filesystem::path(executableDirPath);
        std::ifstream ifs(GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME + "/" + "save.td", std::ios::binary);
        boost::archive::binary_iarchive i(ifs);
        
        /* Here initialize every entity's Tile before assigning moving them to their appopriate position */
        
        td_serialization_collection* collectionToLoadp = new td_serialization_collection();
        i >> *collectionToLoadp;
        
        Map* loadedMap = new Map(collectionToLoadp->serializedMap, spritesVector); // this is our new map!
        std::cout << "Retrieved ENTITIES: " << loadedMap->GetNumberOfEntitiesOfCurrentLevel() << std::endl;
        for (int i = 0; i < loadedMap->GetNumberOfEntitiesOfCurrentLevel(); i++)
        {
            // Maybe make blank entity and set it's attributes?
            
            Actor* newActorComponentp = nullptr;
            unsigned int tileID = collectionToLoadp->entitySerializers[i].spriteIntEnumVal;
            TileSprite tileSpriteEnum = UIntToTileSprite(tileID);
            sf::Sprite sprite = spritesVector.at(static_cast<int>(tileSpriteEnum)); // why do we change it here to int? TODO: Make a method that just accepts an enum
            Tile* restoredEntityTile = new Tile{false, true, sprite, sf::Color::White};
            restoredEntityTile->SetSpriteEnumVal(tileSpriteEnum);
            //if (collectionToLoadp->entitySerializers[i].actor.GetAIType() != AIType::NONE)
            if (collectionToLoadp->entitySerializers[i].actor.GetAIType() != AIType::NONE)
            {
                newActorComponentp = new Actor(collectionToLoadp->entitySerializers[i].actor); // AI set up in the copy constructor
                newActorComponentp->SetupAI(collectionToLoadp->entitySerializers[i].actor.GetAIType());
            }
            Entity* newEntityp = new Entity(collectionToLoadp->entitySerializers[i].entity);
            newEntityp->SetTile(restoredEntityTile);
            newEntityp->SetActorComponent(newActorComponentp);
            newEntityp->SetPosition(newEntityp->GetX(), newEntityp->GetY());
            if (newEntityp->blockingEntitiesVectorPos == 0 && newEntityp->GetName() != "dead")
            {
                /* If something doesn't have an AI set and isn't player it doesn't have the Actor component
                   In other words: only player has an Actor component but doesn't have an AI
                 */
                Actor* playerActorComponent = new Actor();
                playerActorComponent->SetupAI(AIType::NONE);
                newEntityp->SetActorComponent(playerActorComponent);
                engine.SetPlayer(newEntityp);
            }
            std::vector<Entity *>::iterator it;
            it = loadedMap->blockingEntities.begin() + newEntityp->blockingEntitiesVectorPos;
            if (newEntityp->GetName() == "dead")
            {
                std::cout << "WE HAVE A DEAD ENTITY HERE MEDIC XD" << std::endl;
                std::cout << newEntityp->blockingEntitiesVectorPos << std::endl;
                newEntityp->SetActorComponent(nullptr);
            }
//            if (newEntityp->GetActorComponent() != nullptr && newEntityp->GetActorComponent()->GetAI() == nullptr)
//            {
//                newEntityp->SetActorComponent(nullptr);
//            }
            loadedMap->blockingEntities.insert(it, newEntityp);
            std::cout << "Entity: " << newEntityp->GetName() << " Set on (" << newEntityp->GetX() << " " << newEntityp->GetY() << ")" << std::endl;
        }
        engine.LoadGameMap(spritesVector, loadedMap); // move everything here
    }
    
    // Show main menu <- not needed for NOW as it just adds something that takes time
    
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
    EngineState state = engine.mainLoop(window, rng);

    switch (state)
    {
        case EngineState::STATE_EXITING:
            window->close();
            break;
        
        case EngineState::STATE_GAME_OVER:
        {
            window->close();
            if (DestroySavedGameFile())
            {
                std::cout << "Game save file erased" << std::endl;
            }
            break;
        }
            
        case EngineState::STATE_SAVING:
        {
            DestroySavedGameFile(); // the information whether there was a previous save file is useless here.
            std::filesystem::path execPath = std::filesystem::path(executableDirPath);
            std::ofstream ofs(GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME + "/" + "save.td", std::ios::binary);
            boost::archive::binary_oarchive o(ofs);
            
            const Map* map_p = engine.GetGameMap();
            unsigned int numOfEntities = map_p->GetNumberOfEntitiesOfCurrentLevel();
            std::vector<td_entity_serializer> entitySerializers;
            
            /* TODO: Saving immediately after killing the worm doesn't work */
            for (int i = 0; i < numOfEntities; i++)
            {
                Entity* e = map_p->blockingEntities[i]; // If we don't pass a pointer, a copy is made that failes on destructor from td_serializer
                
                if (e != nullptr)
                {
                    const TileSprite ts = e->tile->GetSpriteEnumVal();
                    Entity ec = *e;
                    ec.SetActorComponent(e->GetActorComponent());
                    
                    td_entity_serializer serializer;
                    serializer.SetEntityToSerialize(ec);
                    serializer.SetTileSpriteToSerialize(ts);
                    if (ec.GetActorComponent() != nullptr)
                    {
                        serializer.SetActorToSerialize(*ec.GetActorComponent());
                    }
                    entitySerializers.push_back(serializer);
                } else
                {
                    std::cout << "Dead entity, push serializer with dead entity!" << std::endl;
                    td_entity_serializer serializer;
                    Entity ec;
                    Tile fakeTile{};
                    fakeTile.SetSpriteEnumVal(TileSprite::STAIRS_DOWN);
                    ec.SetTile(&fakeTile);
                    ec.SetName("dead");
                    serializer.SetEntityToSerialize(ec);
                    serializer.SetTileSpriteToSerialize(TileSprite::BRICK_WALL_1);
                    entitySerializers.push_back(serializer);
                }
            }
            
            td_serialization_collection collection{*map_p, entitySerializers}; // when setting up breakpoint here, it !sometimes! crashes, claiming it cannot serialize some data regarding to wall actor fields
            
            std::cout << "SAVED ENTITIES: " << map_p->GetNumberOfEntitiesOfCurrentLevel() << std::endl;
            
            o << collection;
            // TODO: Iterate through levels and save num of entities for each level.
            
            std::cout << "Game saved." << std::endl;
            window->close();
            break;
        }
        default:
            break;
    }
    
    // Engine - Prepare To Exit? Or everything has been prepared?
    // here, engine will free its memory - it goes out of context
}

void App::CreateSaveGameFolder(void)
{
    std::filesystem::path execPath = std::filesystem::path(executableDirPath);
#ifdef __APPLE__
    const std::string fullPath = GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME;
#endif
    
    if (!std::filesystem::exists(fullPath) || (!std::filesystem::is_directory(fullPath)))
    {
        std::filesystem::create_directory(fullPath);
        std::cout << "Save directory created." << std::endl;
    }
}

const std::string App::ReturnSavedGameFilePath(void) const
{
    // only one save game file!
    // pressing "New Game" will erase previous save
    
    std::filesystem::path execPath = std::filesystem::path(executableDirPath);
#ifdef __APPLE__
    const std::string fullPath = GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME;
#endif
    std::string toReturn = "";
    
    if (std::filesystem::exists(fullPath))
    {
        for (const auto& entry : std::filesystem::directory_iterator(fullPath))
        {
            std::string possibleFile = entry.path().string();
            std::cout << possibleFile << std::endl;
            if (possibleFile.find(".td") != std::string::npos)
                toReturn = possibleFile;
        }
    }
    return toReturn;
}

bool App::DestroySavedGameFile(void)
{
    const std::string filePath = ReturnSavedGameFilePath();
    if (std::remove(filePath.c_str()) == 0)
    {
        return true;
    } else
    {
        std::cout << "Couldn't remove file! :" << strerror(errno) << std::endl;
    }
    
    // TODO: Return only std::remove
}
