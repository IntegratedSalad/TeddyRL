#include "app.hpp"
#include "constants.hpp"

#if DEBUG && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().parent_path().parent_path().parent_path().string()
#endif

#if RELEASE && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().string()
#endif

App::App(const std::string execPath) : executableFilePath(execPath)
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
    const std::string pathToSavedGame = ReturnSavedGamePath();
    if (pathToSavedGame.empty())
    {
        CreateSaveGameFolder();
    }
    
    // Show main menu <- not needed for NOW as it just adds something that takes time
    
    EngineState state = engine.mainLoop(window, spritesVector);

    switch (state)
    {
        case EngineState::STATE_EXITING:
            window->close();
            break;
            
        default:
            break;
    }
}

void App::CreateSaveGameFolder(void)
{
    const std::string dirName = "MisioSaves";
    std::filesystem::path execPath = std::filesystem::path(executableFilePath);
#ifdef __APPLE__
    const std::string fullPath = GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + dirName;
#endif
    
    if (!std::filesystem::exists(fullPath) || (!std::filesystem::is_directory(fullPath)))
    {
        std::filesystem::create_directory(fullPath);
    }
}

const std::string App::ReturnSavedGamePath(void) const
{
    // only one save game file!
    // pressing "New Game" will erase previous save
    
    const std::string dirName = "MisioSaves";
    std::filesystem::path execPath = std::filesystem::path(executableFilePath);
#ifdef __APPLE__
    const std::string fullPath = GET_PATH_STR_WORKDIR_MACOS(execPath) + "/" + dirName;
#endif
    std::string toReturn = "";
    
    if (std::filesystem::exists(fullPath))
    {
        std::string filePath = "";
        
        for (const auto& entry : std::filesystem::directory_iterator(fullPath))
        {
            toReturn = entry.path();
            break; // return first file
        }
    }
    return toReturn;
}
