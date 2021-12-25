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

class Entity
{
private:
    
    Tile* tile;
    
    Actor* actorComponent;
    
    int x;
    int y;
    
public:
    
    Entity();
    Entity(Tile* _tile, int _x, int _y, bool);
    Entity(Tile* _tile, int _x, int _y, bool, Actor*);
    
    ~Entity();
    
    void move(int moveX, int moveY);
    
    void draw();
    
    bool isEntityBlocking(void)
    {
        return tile->canBlock;
    }
    
};

#endif /* entity_hpp */
