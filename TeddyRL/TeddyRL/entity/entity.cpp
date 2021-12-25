//
//  entity.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "entity.hpp"

Entity::Entity() : x(0), y(0), actorComponent(nullptr)
{
    
}

Entity::Entity(Tile* _tile, int _x, int _y, bool blocks) : x(_x), y(_y), tile(_tile), actorComponent(nullptr)
{
    
}

Entity::Entity(Tile* _tile, int _x, int _y, bool blocks, Actor* comp) : x(_x), y(_y), tile(_tile), actorComponent(comp)
{

}