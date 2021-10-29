#include "engine.hpp"


Engine::Engine()
{
    isRunning = false;
}

void Engine::mainLoop(std::shared_ptr<sf::RenderWindow> window)
{   

    sf::Font font;
    sf::Text text;

    if (!font.loadFromFile("dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        window->close();

    }

    text.setString("Dupa");

    text.setCharacterSize(10);

    text.setFillColor(sf::Color::Red);


    while (window->isOpen())
    {

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }
    
        window->clear(sf::Color::Black);
        window->draw(text);
        window->display();
    }
    

}