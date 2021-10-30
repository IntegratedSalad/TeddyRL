#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class Engine
{
private:
    bool isRunning;

public:
    Engine();
    void mainLoop(sf::RenderWindow* window);
};
