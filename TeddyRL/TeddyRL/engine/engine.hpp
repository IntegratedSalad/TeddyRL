#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#ifdef __APPLE__
#include "../ResourcePath.hpp"
#endif


/* TODO: all caps */
enum class EngineState
{
    STATE_running = 0,
    STATE_saving,
    STATE_exiting,
    STATE_menu
    
};

class Engine
{
private:
    bool isRunning;

public:
    Engine();
    EngineState mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> tilesetVector);
};

