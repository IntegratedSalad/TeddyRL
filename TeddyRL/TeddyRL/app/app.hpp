#include <SFML/Graphics.hpp>
#include "../engine/engine.hpp"
#include "Tileset.hpp"

class App
{
    /* Handles all loading/setting up tasks as e.g. setting up windows, tileset, fonts, music, savefiles etc. */

private:
    sf::RenderWindow* window;
    
    const std::vector<sf::Sprite> tilesetVector;
    const std::vector<sf::Sprite> setTileSetVector(Tileset tileset);
    
public:
    
    App();
    ~App();
    void run();
    Tileset setTileset(const std::string tilesetPath);

};


// TODO:
// Manage resource path ifdef...
