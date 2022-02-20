//
//  entity.hpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef entity_hpp
#define entity_hpp

#include <iostream>
#include <memory>
#include "tile.hpp"
#include "actor.hpp"
#include "constants.hpp"

/* Entity is a single object on game map, that isn't a free space.
   Entity has a pointer to Tile object, that is its graphical representation.
 */

class Entity
{
private:
    
    Actor* actorComponent;
    
    int x;
    int y;
    
public:
    
    Tile* tile;
    
    Entity();
    Entity(Tile* _tile, int _x, int _y);
    Entity(Tile* _tile, int _x, int _y, Actor*);
    
    ~Entity();

    void move(int moveX, int moveY);
    
//    void draw();
    
    bool isEntityBlocking(void)
    {
        return tile->canBlock;
    }
    
    void setPosition(int _x, int _y);
    void setX(int);
    void setY(int);
    
};

#endif /* entity_hpp */
