#include "app.hpp"
#include "constants.hpp"

App::App()
{
    sf::VideoMode videoMode = sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT);
    window = new sf::RenderWindow(videoMode, "TeddyRL");
    
    window->setFramerateLimit(60);
    window->setKeyRepeatEnabled(false);

}

App::~App()
{
    delete this->window;
}

void App::run()
{
    Tileset tileset("tileset.png", C_TILE_TILESET_SIZE);
    
    const std::vector<sf::Sprite> spritesVector = tileset.getSprites();
//
//    if (!font->loadFromFile(resourcePath() + "dos_vga_font.ttf"))
//    {
//        std::cout << "Couldn't load the font. Exiting" << std::endl;
//
//        return EngineState::STATE_EXITING;
//
//    }
    
    Engine engine = Engine();
    EngineState state = engine.mainLoop(window, spritesVector);
//    engine.gameFont = this->font;
//    engine.gameFont = sf::Font(font);
    
    

    switch (state)
    {
        case EngineState::STATE_EXITING:
            window->close();
            break;
            
        default:
            break;
    }
}
