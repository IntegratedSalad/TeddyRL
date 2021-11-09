//
//  Tileset.cpp
//  TeddyRL
//
//  Created by Dev on 31/10/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "Tileset.hpp"

Tileset::Tileset()
{
    
}

Tileset::~Tileset()
{
    
}

Tileset::Tileset(const std::string _fileName, const int _tileSize)
{
    std::cout << _fileName << std::endl;

    if (!tilesetImage.loadFromFile(resourcePath() + _fileName))
    {
        std::cout << "Couldn't load tileset file " << _fileName << std::endl;
        exit(-1);
    }

    
    tileSize = _tileSize;
    fileName = _fileName;
    
    getSpritesFromTilesetImage(&spriteVector, _tileSize);
    
    
}

/*
 
 WHEN CREATING AN ACTOR CLASS, THINK ABOUT HOW WE ARE GOING TO GIVE ACCESS
 TO THIS SPRITE VECTOR. WE DON'T WANT TO MAKE COPIES AND WE DON'T WANT ACTOR CLASS
 TO MODIFY THIS VECTOR.
 
 */


void Tileset::getSpritesFromTilesetImage(std::vector<sf::Sprite>* vec, const int tileSize)
{
    
    std::cout << "Tileset image dimensions:" << std::endl;
    std::cout << tilesetImage.getSize().x << tilesetImage.getSize().y << std::endl;
//    exit(0);
    
    const sf::Vector2u IMAGESIZE = this->tilesetImage.getSize();
    
//    std::cout << IMAGESIZE.x << std::endl;
    std::cout << IMAGESIZE.y / tileSize << std::endl;
    
    for (int _y = 0; _y < IMAGESIZE.y; _y += tileSize)
    {
        for (int _x = 0; _x < IMAGESIZE.x; _x += tileSize)
        {
            sf::Texture* texture = new sf::Texture();
            if (!texture->loadFromFile(resourcePath() + fileName, sf::IntRect(_x, _y, tileSize, tileSize)))
            {
            }
            
            sf::Sprite sprite;
            
            sprite.setTexture(*texture);
            
            vec->push_back(sprite);
        }
    }
    // REMEMBER TO DELETE TEXTURES AFTER PROGRAM EXITS!
}

