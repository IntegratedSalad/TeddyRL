#include "app.hpp"
#include "td_serializers.hpp"
#include "constants.hpp"

#if DEBUG && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().parent_path().parent_path().parent_path().string()
#endif

#if RELEASE && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().string()
#endif

const std::string SAVE_DIR_NAME = "MisioSaves";

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
        Map* mp = new Map(spritesVector);

        std::filesystem::path execPath = std::filesystem::path(executableDirPath);
        std::ifstream ifs(GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME + "/" + "save.td", std::ios::binary);
        boost::archive::binary_iarchive i(ifs);
        
        /* TESTING LOADING */
        
        /* Here initialize every entity's Tile before assigning moving them to their appopriate position */
        
        td_serialization_collection collectionToLoad;
        i >> collectionToLoad;
        
        Map loadedMap = collectionToLoad.serializedMap;

        for (int i = 0; i < loadedMap.GetNumberOfEntitiesOfCurrentLevel(); i++)
        {
            Entity ec = collectionToLoad.entitySerializers[i].entity;
            std::cout << "NAME OF RETRIEVED ENTITY: " << ec.getName() << std::endl;
            
        }
        
        /*
         It tries to load tiles, which are not saved. Either we save everything, or we don't save tiles and we handle loading the tiles separately.
         If there are multiple levels, each level can add up to the size of the save,  of the saved image
         Perhaps we have to be more specific and save and restore every entity individually.
         */
        engine.LoadGameMap(spritesVector, mp);
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
            
        case EngineState::STATE_SAVING:
        {
            std::filesystem::path execPath = std::filesystem::path(executableDirPath);
            std::ofstream ofs(GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME + "/" + "save.td", std::ios::binary); // TODO: Extract path once and save it
            boost::archive::binary_oarchive o(ofs);
            
            const Map* map_p = engine.GetGameMap();
            unsigned int numOfEntities = map_p->GetNumberOfEntitiesOfCurrentLevel();
            std::vector<td_entity_serializer> entitySerializers;
            /* We don't save the entity vector. Although we only need number of entities while we deserialize each entity, we can iterate on that vector and create a serializer for each entity, let's keep things consistent. */

            for (int i = 0; i < numOfEntities; i++)
            {
                Entity* e = map_p->blockingEntities[i]; // If we don;t pass a pointer, a copy is made that failes on destructor from td_serializer
                const TileSprite ts = e->tile->GetSpriteEnumVal();
                
                Entity ec = *e;
                
                td_entity_serializer serializer{ec, ts};
                entitySerializers.push_back(serializer);
            }
            
            td_serialization_collection collection{*map_p, entitySerializers};
            
            std::cout << "SAVED ENTITIES: " << map_p->GetNumberOfEntitiesOfCurrentLevel() << std::endl;
            
            o << collection;

            //o << // pass gameMaps's entity vector to td_entity_serializer
            // it will serialize a vector of structs that have entity and its tilesprite.
            // then deserializing entities and making Map's blockingEntities vector will be done manually.
            // TODO: Write down this process
             
            // Iterate through levels and save num of entities for each level.
            
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

void App::DestroySavedGameFile(void)
{
    
}
