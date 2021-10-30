#include "engine.hpp"
#include "ResourcePath.hpp"

Engine::Engine()
{
    isRunning = false;
}

void Engine::mainLoop(sf::RenderWindow* window)
{

    sf::Font font;
    sf::Text text;

    if (!font.loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        window->close();

    }

    text.setString("Dupadsdsds");
    text.setFont(font);

    text.setCharacterSize(20);

    text.setFillColor(sf::Color::White);

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

        window->clear();
        window->draw(text);
        window->display();
    }
    

}
