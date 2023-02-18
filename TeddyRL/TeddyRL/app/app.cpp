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
    
    // Maybe here add class SpriteColection with a getter
    
    Engine engine = Engine();
    
    // Show main menu <- not needed for now as it just adds something that takes time
    // Check if there is no save games
    // engine.checkForSaveGame()
    
    EngineState state = engine.mainLoop(window, spritesVector);

    switch (state)
    {
        case EngineState::STATE_EXITING:
            window->close();
            break;
            
        default:
            break;
    }
}
