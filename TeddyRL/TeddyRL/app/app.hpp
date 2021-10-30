#include <SFML/Graphics.hpp>
#include "../engine/engine.hpp"
#include <memory>

class App
{
    /* Handles all loading/setting up tasks as e.g. setting up windows, tileset, fonts, music etc. */

private:
    sf::RenderWindow* window;

public:
    App();
    ~App();

    // void setWindow(sf::RenderWindow* w)
    // {
    //     window = w;
    // }

    void run();

};
