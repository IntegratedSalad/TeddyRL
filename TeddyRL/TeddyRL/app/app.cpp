#include "app.hpp"
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
    
    if (pathToSavedGameFile.empty())
    {
        CreateSaveGameFolder();
        engine.SetupNewGameMap(spritesVector);
    } else
    {
        Map* mp = new Map(spritesVector);
        
        // Maybe we have to initialize everything here? Bad access seems to occur because every field in map class has not have allocated memory
        
        // Or we have to be more specific and save and restore every entity individually.
        
        std::filesystem::path execPath = std::filesystem::path(executableDirPath);
        std::ifstream ifs(GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + SAVE_DIR_NAME + "/" + "save.td", std::ios::binary);
        boost::archive::binary_iarchive i(ifs);
        i >> *mp; // BAD ACCESS
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
            o << *(engine.GetGameMap());
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
