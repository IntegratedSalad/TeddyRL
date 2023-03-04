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
        
        for (int i = 0; i < loadedMap->GetNumberOfEntitiesOfCurrentLevel(); i++)
        {
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
            }
            Entity* newEntityp = new Entity(collectionToLoadp->entitySerializers[i].entity);
            newEntityp->SetTile(restoredEntityTile);
            newEntityp->setActorComponent(newActorComponentp);
            newEntityp->setPosition(newEntityp->getX(), newEntityp->getY());
            if (newEntityp->blockingEntitiesVectorPos == 0)
            {
                /* If something doesn't have an AI set and isn't player it doesn't have the Actor component
                   In other words: only player has an Actor component but doesn't have an AI
                 */
                Actor* playerActorComponent = new Actor();
                playerActorComponent->SetupAI(AIType::NONE);
                newEntityp->setActorComponent(playerActorComponent);
                engine.SetPlayer(newEntityp);
            }
            std::vector<Entity *>::iterator it;
            it = loadedMap->blockingEntities.begin() + newEntityp->blockingEntitiesVectorPos;
            loadedMap->blockingEntities.insert(it, newEntityp);
            
            std::cout << "Entity: " << newEntityp->getName() << " Set on (" << newEntityp->getX() << " " << newEntityp->getY() << ")" << std::endl;
            
            if (newEntityp->getName() == "Worm")
            {
                std::cout << "Worm!" << std::endl;
            }
        }
//
//        std::vector<Entity* > newVectorOfBlockingEntities; // this makes a copy!!!
//
//        for (int i = 0; i < loadedMap->GetNumberOfEntitiesOfCurrentLevel(); i++)
//        {
//            /* Use copy constructors */
////            Entity* newEntityp = new Entity{};
////            Actor* acp = new Actor{};
//
//            /* Get entity copy */
//            Entity ec = collectionToLoad.entitySerializers[i].entity;
//            /* Get tile ID */
//            unsigned int tileID = collectionToLoad.entitySerializers[i].spriteIntEnumVal;
//            /* Get tile sprite */
//            TileSprite tileSpriteEnum = UIntToTileSprite(tileID);
//            sf::Sprite sprite = spritesVector.at(static_cast<int>(tileSpriteEnum));
//            /*  Get actor copy */
//            Actor ac = collectionToLoad.entitySerializers[i].actor;
//            /* Set tile */
//            Tile* restoredEntityTile = new Tile{false, true, sprite, sf::Color::White}; // TODO: Make static method or constructor. Or add .Create() method, which utilizes given TileSprite and options
//            ec.SetTile(restoredEntityTile);
//            /* Set actor and AI based on AIType */
//            ac.SetupAI(collectionToLoad.entitySerializers[i].actor.GetType());
//            ec.setActorComponent(&ac);
//
//            //acp = &ac; // points always to the same place in memory
//
//            //ec.setActorComponent(acp);
//            /* Point to entity copy */
//
//            //newEntityp = &ec;
//
//            Actor* newActorp = new Actor();
//            Entity* newEntityp = new Entity();
//
//            // Let's try to manually set fields of these objects
//
//            newActorp->SetupAI(ac.GetType());
//            newEntityp->SetTile(restoredEntityTile);
//            newEntityp->setActorComponent(newActorp);
//
//            /*                      */
//            if (ec.blockingEntitiesVectorPos == 0) // we have the player
//            {
//                engine.SetPlayer(newEntityp);
//            }
//            /* Push entity to newVectorOfBlockingEntities at position of blockingEntitiesVectorPos */
//            std::vector<Entity* >::iterator it;
//            it = newVectorOfBlockingEntities.begin() + newEntityp->blockingEntitiesVectorPos;
//            newVectorOfBlockingEntities.insert(it, newEntityp);
//            /* */
//
//            /* Setup */
//
//            /*
//             1. Set all Entities:
//
//                1. Get Entity copy
//                2. Get TileID
//                3. Get TileSprite
//                4. Get Sprite
//                5. Get Actor
//                6. Set Tile
//                7. Set Actor and AI based on AIType.
//                8. Push entity to newVectorOfBlockingEntities at position of blockingEntitiesVectorPos
//
//             2. Set newVectorOfBlockingEntities as loadedMap's blockingEntities
//             3. Point mp to loadedMap.
//             */
//
//        }
//        //loadedMap->blockingEntities = newVectorOfBlockingEntities;
//
//        Map* mp = new Map(*loadedMap);
//        mp->blockingEntities = newVectorOfBlockingEntities;
//        //mp = &loadedMap; // maybe try to make a copy constructor
        
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
            std::ofstream ofs(GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME + "/" + "save.td", std::ios::binary); // TODO: Extract path once and save it
            boost::archive::binary_oarchive o(ofs);
            
            const Map* map_p = engine.GetGameMap();
            unsigned int numOfEntities = map_p->GetNumberOfEntitiesOfCurrentLevel();
            std::vector<td_entity_serializer> entitySerializers;
            /* We don't save the entity vector. Although we only need number of entities while we deserialize each entity, we can iterate on that vector and create a serializer for each entity, let's keep things consistent. */
            
            /* TODO: Saving immediately after killing the worm doesn't work */
            for (int i = 0; i < numOfEntities; i++)
            {
                Entity* e = map_p->blockingEntities[i]; // If we don;t pass a pointer, a copy is made that failes on destructor from td_serializer
                const TileSprite ts = e->tile->GetSpriteEnumVal();
                Entity ec = *e;
                ec.setActorComponent(e->getActorComponent());
                
                td_entity_serializer serializer;
                serializer.SetEntityToSerialize(ec);
                serializer.SetTileSpriteToSerialize(ts);
                if (ec.getActorComponent() != nullptr)
                {
                    serializer.SetActorToSerialize(*ec.getActorComponent());
                }
                entitySerializers.push_back(serializer);
            }
            
            td_serialization_collection collection{*map_p, entitySerializers};
            
            std::cout << "SAVED ENTITIES: " << map_p->GetNumberOfEntitiesOfCurrentLevel() << std::endl;
            
            o << collection;

            //o << // pass gameMaps's entity vector to td_entity_serializer
            // it will serialize a vector of structs that have entity and its tilesprite.
            // then deserializing entities and making Map's blockingEntities vector will be done manually.
             
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
}
