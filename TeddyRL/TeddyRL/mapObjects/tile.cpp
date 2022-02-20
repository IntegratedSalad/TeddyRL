//
//  tile.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "tile.hpp"

/* Sprite should be a shared pointer, because there will be multiple instances of tiles using the same sprite. Not even a shared pointer - it should be a copy. */

Tile::Tile() : sprite(), isInvisible(false), canBlock(false)
{
    tileColor = sf::Color();
}

Tile::Tile(bool isInvisible, bool blocks, sf::Sprite sprite, sf::Color color)
     : isInvisible(isInvisible), canBlock(blocks)
{
    this->setTile(sprite, color);
    this->setSize(sf::Vector2f(C_TILE_IN_GAME_SIZE, C_TILE_IN_GAME_SIZE));
    
}

Tile::Tile(bool blocks) : sprite(), isInvisible(true)
{
    
}

Tile::Tile(bool isInvisible, bool blocks) : isInvisible(isInvisible), canBlock(blocks)
{
    
}

Tile::~Tile()
{
}

/* setTile sets a colored sprite to the tile and sets the alpha mask of it */

void Tile::setTile(sf::Sprite& _sprite, sf::Color color)
{
    
    if (this->isInvisible)
    {
        _sprite.setColor(sf::Color::Transparent);
        
    } else
    {
        _sprite.setColor(color);
        
    }
    
    const sf::Texture* t = new sf::Texture();
    t = _sprite.getTexture();
    
    this->setTexture(t);
    this->setFillColor(color);
}


void Tile::makeVisible()
{
    this->isInvisible = false;
    this->sprite.setColor(this->tileColor);
}


void Tile::makeHidden()
{
    this->isInvisible = true;
    this->sprite.setColor(sf::Color::Transparent);
}
