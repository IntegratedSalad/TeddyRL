#include <SFML/Graphics.hpp>
#include "engine.hpp"
#include "tileset.hpp"

class App
{
    /* Handles all loading/setting up tasks as e.g. setting up windows, tileset, fonts, music, savefiles etc. */

private:
    sf::RenderWindow* window;
    sf::Font font;
    
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
