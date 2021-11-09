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

    sf::Image tilesetImage;
    
    void getSpritesFromTilesetImage(std::vector<sf::Sprite>* vec, const int tileSize);
    
public:

    Tileset();
    ~Tileset();
    
    Tileset(const std::string _fileName, const int _tileSize);
    
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
    
    const std::vector<sf::Sprite> getTiles(void)
    {
        return spriteVector;
    }
    
    void removeBackgroundFromTilesetImage(sf::Image& img, const sf::Color color);
    
    
};

enum class TileSprite
{
    
};

#endif /* Tileset_hpp */
