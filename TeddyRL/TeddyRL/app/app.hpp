#include <SFML/Graphics.hpp>
#include "engine.hpp"
#include "tileset.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

class App
{
    /* Handles all loading/setting up tasks as e.g. setting up windows, tileset, fonts, music, savefiles etc. */

private:
    sf::RenderWindow* window;
    sf::Font* font;
    
    const std::string executableDirPath;
    
    const std::vector<sf::Sprite> tilesetVector;
    const std::vector<sf::Sprite> setTileSetVector(Tileset tileset);
    
    void CreateSaveGameFolder(void);
    void HandleExit();
    const std::string ReturnSavedGameFilePath(void) const;
    void DestroySavedGameFile(void);
    
public:
    
    App(const std::string execPath);
    
    ~App();
    void run();
    Tileset setTileset(const std::string tilesetPath);
};


// TODO:
// Manage resource path ifdef...
