//
//  Tileset.hpp
//  TeddyRL
//
//  Created by Dev on 31/10/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef Tileset_hpp
#define Tileset_hpp
#include <SFML/Graphics.hpp>
#include <vector>

#include "ResourcePath.hpp"
#include <iostream>

class Tileset
{
private:
    
    std::string fileName;
    int tileSize;
    
    std::vector<sf::Sprite> spriteVector;
    
    /* Image to load the tileset from */
    sf::Image tilesetImage;
    
    void GetSpritesFromTilesetImage(std::vector<sf::Sprite>* vec, const int tileSize);
    
public:

    Tileset();
    Tileset(const std::string _fileName, const int _tileSize);
    
    ~Tileset();
    
    const int getTileSize(void)
    {
        return tileSize;
    }
    
    void setTileSize(const int tS)
    {
        tileSize = tS;
    }
    
    const std::string getFileName(void)
    {
        return fileName;
    }
    
    void setFileName(const std::string fN)
    {
        fileName = fN;
    }
    
    const std::vector<sf::Sprite> getSprites(void) // TODO: Is this should be a pointer? we essentialy are making copies of sf::Sprites that have allocated textures
    {
        return spriteVector;
    }
    
    void removeBackgroundFromTilesetImage(sf::Image& img, const sf::Color color);
    
};

class SpriteCollection
{
private:
    static std::vector<sf::Sprite> spriteVector;
    
public:
    static sf::Sprite Get();
};

#endif /* Tileset_hpp */
