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
        
        td_serialization_collection* collectionToLoadp = new td_serialization_collection();
        i >> *collectionToLoadp;
        
        Map* loadedMap = new Map(collectionToLoadp->serializedMap, spritesVector); // this is our new map!
        std::cout << "Retrieved ENTITIES: " << loadedMap->GetNumberOfEntitiesOfCurrentLevel() << std::endl;
        for (int i = 0; i < collectionToLoadp->entitySerializers.size(); i++)
        {
         // TODO: We have to standardize object (Entity) creation and setting its fields in the constructor!
            // Get and set AI.
            // Get and set actor
            // Get and set Tile
            // TODO: Define a constructor that takes these three things and properly sets the Entity.
            // Exactly : everything has to be done in a copy constructor.
            // Copy constructor needs to be defined for map as well.
            
            if (collectionToLoadp->entitySerializers[i].entity.blockingEntitiesVectorPos == 0)
            {
                std::cout << collectionToLoadp->entitySerializers[i].entity.GetName() << std::endl;
            }
            
            if (collectionToLoadp->entitySerializers[i].entity.GetName() != "dead") // delete this condition
            {
                Actor* actorp = new Actor(collectionToLoadp->entitySerializers[i].actor);
                Tile* tilep = new Tile(collectionToLoadp->entitySerializers[i].spriteIntEnumVal, false, true, sf::Color::White, spritesVector);
                
                Entity recoveredEntity = collectionToLoadp->entitySerializers[i].entity;
//                recoveredEntity.SetActorComponent(actorp);
//                recoveredEntity.SetTile(tilep);
                
                Entity* newEntityp = new Entity(recoveredEntity);
                
                if (collectionToLoadp->entitySerializers[i].FLAGS & TD_SER_NEEDS_ACTOR_COMP)
                {
                    newEntityp->SetActorComponent(actorp);
                } else
                {
                    delete actorp;
                }
                
                newEntityp->SetTile(tilep);
                newEntityp->tile->move(newEntityp->GetX() * C_TILE_IN_GAME_SIZE, newEntityp->GetY() * C_TILE_IN_GAME_SIZE);
                loadedMap->LoadBlockingEntityBackOnMap(newEntityp);
            }
        }
        engine.LoadGameMap(spritesVector, loadedMap);
        engine.SetPlayer();
        delete collectionToLoadp;
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
  
            engine.ClearGameMap();
            
//            std::cout << "numOfEntities" << numOfEntities << std::endl;
//            std::cout << "size of blockingEntities" << map_p->blockingEntities.size() << std::endl;
//
            const Map* map_p = engine.GetGameMap();
            unsigned int numOfEntities = map_p->GetNumberOfEntitiesOfCurrentLevel(); // useless
            std::vector<td_entity_serializer> entitySerializers;
            
            /* TODO: Saving immediately after killing the worm doesn't work */
            // check what entities are saved, and how many are there.
            // check if any weirdly killed entities are saved.
            
            // TODO: Update blockingEntitiesVector and Int2DVector
            
            for (int i = 0; i < engine.GetGameMap()->blockingEntities.size(); i++)
            {
                Entity* e = map_p->blockingEntities[i]; // If we don't pass a pointer, a copy is made that fails on destructor from td_serializer
                
                // TODO: Add td_entity_serializer's "needsActorComponent".
                //       In that way, we know we should initialize it.
                // Maybe this thing has something with handling how the monsters die?
                // I think I didn't think through how the enemies are handled when they die.
                // How they are handled in terms of beign wiped from the map.
                
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
                        serializer.FLAGS |= TD_SER_NEEDS_ACTOR_COMP;
                        serializer.SetActorToSerialize(*ec.GetActorComponent());
                    }
                    entitySerializers.push_back(serializer);
                } else
                {
                    std::cout << "Fatal error - nullptr in blockingEntities after cleanup!" << std::endl;
                    exit(0);
                }
            }
            
            td_serialization_collection collection{*map_p, entitySerializers}; // when setting up breakpoint here, it !sometimes! crashes, claiming it cannot serialize some data regarding to wall actor fields
            
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
