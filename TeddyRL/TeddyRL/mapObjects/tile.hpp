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

/* Tile is a structure that has a colored sprite. */

typedef struct Tile
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

    
private:
    
    sf::Sprite sprite; // should be a copy
    sf::Color tileColor;
    
    void setTile(sf::Sprite sprite, sf::Color color);
    
    
} Tile;

#endif /* tile_hpp */
