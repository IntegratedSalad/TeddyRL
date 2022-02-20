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

Entity::Entity(Tile* _tile, int _x, int _y) : x(_x), y(_y), tile(_tile), actorComponent(nullptr)
{
    
}

Entity::Entity(Tile* _tile, int _x, int _y, Actor* comp) : x(_x), y(_y), tile(_tile), actorComponent(comp)
{

}

/* TODO: While moving we will be erasing air entities (free space). We should update this somehow. Swap entities? */
void Entity::move(int moveX, int moveY) // later add entity vector
{
    
    this->tile->move(moveX, moveY);
    this->x = this->tile->getPosition().x;
    this->y = this->tile->getPosition().y;
}

void Entity::setPosition(int _x, int _y)
{
    this->x = _x;
    this->y = _y;
    
    this->tile->setPosition(_x * C_TILE_TILESET_SIZE, _y * C_TILE_TILESET_SIZE);
}

void Entity::setX(int _x)
{
    this->x = _x;
}

void Entity::setY(int _y)
{
    this->y = _y;
}

Entity::~Entity()
{
    delete this->tile;
}
