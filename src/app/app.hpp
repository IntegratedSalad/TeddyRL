#include <SFML/Graphics.hpp>
#include "engine/engine.hpp"
#include <memory>

class App
{
    /* Handles all loading/setting up tasks as e.g. setting up windows, tileset, fonts, music etc. */

private:
    std::shared_ptr<sf::RenderWindow> window;

public:
    App();
    ~App();

    // void setWindow(sf::RenderWindow* w)
    // {
    //     window = w;
    // }

    void run();

};
