//
//  tile.hpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef tile_hpp
#define tile_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"

/* Tile is a structure that has a colored sprite.
   It inherits from sf::Rectangle Shape that allows it to be drawn and transformed.
 
 */

typedef struct Tile : public sf::RectangleShape
{
    
    bool canBlock;
    bool isInvisible;
    
    Tile(bool isInvisible, bool blocks);
    Tile(bool isInvisible, bool blocks, sf::Sprite sprite, sf::Color color);
    Tile(bool blocks);
    Tile();
    
    ~Tile();
    
    void makeVisible();
    void makeHidden();
    
    void setSprite(sf::Sprite sprite) { this->sprite = sprite; };
    void setTile(sf::Sprite& sprite, sf::Color color);

private:
    
    sf::Sprite sprite;
    sf::Color tileColor;
    
} Tile;

enum class TileSprite
{
    EMPTY_SPACE = 15,
    MAN = 0,
    TEDDY = 73,
    SNAKE = 70,
    
    BRICK_WALL_1 = 128,
    STAIRS_DOWN = 138,
    STAIRS_UP = 139,
    
    CORPSE = 225,
    
};

#endif /* tile_hpp */
