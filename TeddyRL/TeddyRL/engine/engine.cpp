#include "engine.hpp"

Engine::Engine()
{
    isRunning = false;
}

EngineState Engine::mainLoop(sf::RenderWindow* window, const std::vector<sf::Sprite> tilesetVector)
{

    sf::Font font;
    sf::Text text;

    if (!font.loadFromFile(resourcePath() + "dos_vga_font.ttf"))
    {
        std::cout << "Couldn't load the font. Exiting" << std::endl;
        
        return EngineState::STATE_exiting;

    }
    
    text.setString("Dupadsdsds");
    text.setFont(font);

    text.setCharacterSize(20);

    text.setFillColor(sf::Color::White);

    while (window->isOpen()) // isRunning
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

        
        int x = 0;
        int y = 0;
        for (int i = 0; i < tilesetVector.size(); i++)
        {
            sf::Sprite currentSprite;
            currentSprite = tilesetVector.at(i);
            
            currentSprite.move(sf::Vector2f(x, y));
            
            x += 16;
            
            if (x >= 16 * 10)
            {
                y += 16;
                x = 0;
            }
            
            window->draw(currentSprite);
        }
    
//
//        sf::Sprite sprite = tilesetVector.at(7);
//        sf::Sprite second_sprite = tilesetVector.at(9);
//        second_sprite.move(16, 16);
//
//        window->draw(sprite);
//        window->draw(second_sprite);
//
        
//        window->draw(text);
        window->display();
    }
    

}
