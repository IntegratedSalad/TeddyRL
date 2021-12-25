//
//  Tileset.cpp
//  TeddyRL
//
//  Created by Dev on 31/10/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "tileset.hpp"

Tileset::Tileset()
{
    
}

Tileset::~Tileset()
{
    for (int i = 0; i < spriteVector.size(); i++)
    {
        delete spriteVector[i].getTexture();
    }
}

Tileset::Tileset(const std::string _fileName, const int _tileSize)
{
    std::cout << _fileName << std::endl;
    
    const sf::Color backgroundColor = sf::Color(17, 17, 51);
    const sf::Color dotBackgroundColor = sf::Color(51, 51, 85);
    
    if (!tilesetImage.loadFromFile(resourcePath() + _fileName))
    {
        std::cout << "Couldn't load tileset file " << _fileName << std::endl;
        exit(-1);
    }

    removeBackgroundFromTilesetImage(tilesetImage, backgroundColor);
    removeBackgroundFromTilesetImage(tilesetImage, dotBackgroundColor);
    
    tileSize = _tileSize;
    fileName = _fileName;
    
    getSpritesFromTilesetImage(&spriteVector, _tileSize);
    
}

/*
 
 WHEN CREATING AN ACTOR CLASS, THINK ABOUT HOW WE ARE GOING TO GIVE ACCESS
 TO THIS SPRITE VECTOR. WE DON'T WANT TO MAKE COPIES AND WE DON'T WANT ACTOR CLASS
 TO MODIFY THIS VECTOR.
 
 */

/* We should probably make copies if we want to customize the sprites. */


void Tileset::getSpritesFromTilesetImage(std::vector<sf::Sprite>* vec, const int tileSize)
{

    
    std::cout << "Tileset image dimensions:" << std::endl;
    std::cout << tilesetImage.getSize().x << tilesetImage.getSize().y << std::endl;

    const sf::Vector2u IMAGESIZE = this->tilesetImage.getSize();

    for (int _y = 0; _y < IMAGESIZE.y; _y += tileSize)
    {
        for (int _x = 0; _x < IMAGESIZE.x; _x += tileSize)
        {
            sf::Texture* texture = new sf::Texture();
            texture->loadFromImage(tilesetImage, sf::IntRect(_x, _y, tileSize, tileSize));
            sf::Sprite sprite;
            
            sprite.setTexture(*texture);
            
            vec->push_back(sprite);
        }
    }
}

void Tileset::removeBackgroundFromTilesetImage(sf::Image& img, const sf::Color color)
{
    img.createMaskFromColor(color);
}
