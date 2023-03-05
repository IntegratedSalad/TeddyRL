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

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

/* Tile is a structure that has a colored sprite.
   It inherits from sf::Rectangle Shape that allows it to be drawn and transformed.
 
 */

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

typedef struct Tile : public sf::RectangleShape
{
    
    // TODO: We have to save everything but the sprite (not in this class), and make Tile from scratch when loading.
    
    bool canBlock;
    bool isInvisible;
    
    Tile(bool isInvisible, bool blocks);
    Tile(bool isInvisible, bool blocks, sf::Sprite sprite, sf::Color color);
    Tile(bool blocks);
    Tile();
    
    Tile(TileSprite, bool isInvisible, bool blocks, sf::Color, const std::vector<sf::Sprite> spritesVector); // make use of that
    
    ~Tile();
    
    void MakeVisible();
    void MakeHidden();
    
    void SetSprite(sf::Sprite sprite) { this->sprite = sprite; };
    void SetTexture(sf::Sprite& sprite, sf::Color color);
    
    TileSprite GetSpriteEnumVal(void) { return this->spriteEnumVal; };
    void SetSpriteEnumVal(TileSprite ts) {this->spriteEnumVal = ts; };

private:
    
    TileSprite spriteEnumVal;
    sf::Sprite sprite;
    sf::Color tileColor;
    
} Tile;

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, TileSprite ts, const unsigned int version)
{
    ar & ts;
}

} // namespace serialization
} // namespace boost


#endif /* tile_hpp */
