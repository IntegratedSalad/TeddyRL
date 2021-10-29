#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class Engine
{
private:
    bool isRunning;

public:
    Engine();
    void mainLoop(std::shared_ptr<sf::RenderWindow> window);
};
