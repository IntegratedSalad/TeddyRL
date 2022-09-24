//
//  entity.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "entity.hpp"
#include <iostream>

Entity::Entity() : x(0), y(0), actorComponent(nullptr), mapVectorPos(0)
{
    this->setPosition(x, y);
}

Entity::Entity(Tile* _tile, int _x, int _y) : x(_x), y(_y), tile(_tile), actorComponent(nullptr), mapVectorPos(0)
{
    this->setPosition(x, y);
}

Entity::Entity(Tile* _tile, int _x, int _y, Actor* comp) : x(_x), y(_y), tile(_tile), actorComponent(comp), mapVectorPos(0)
{
    this->setPosition(x, y);
}


/* TODO: Later it should return something telling us about attacking etc. */
// we have to pass map instance, not intVec and entity Vec
#warning Maybe we shouldn't use map structures to perform logic on entities but built in methods for distance etc. We should use rects for collision.
void Entity::move(int moveX, int moveY, Int2DVec& intVec, std::vector<Entity* > entityVector)
{
    int spaceArrayIndex = intVec[this->x + moveX][this->y + moveY];
    Entity* playerPointer = entityVector[0];
    
    std::cout << "SPI: " << spaceArrayIndex << std::endl;

    
    /* Use rect to detect collision then get entity at position of collision. What index is in intVec, it's an index to the entity in entityVector.
     
     */
    
    if (spaceArrayIndex < 0) // shouldn't be used to detect collision
    {
        intVec[this->x][this->y] = -1;
        /* Tile transformation */
        moveX *= C_TILE_IN_GAME_SIZE;
        moveY *= C_TILE_IN_GAME_SIZE;
            
        this->tile->move(moveX, moveY);
        /*                     */
        
        this->x = this->tile->getPosition().x / C_TILE_IN_GAME_SIZE;
        this->y = this->tile->getPosition().y / C_TILE_IN_GAME_SIZE;
        
        intVec[this->x][this->y] = this->entityVectorPos;
    }
    else // return entity and decide what to do with it.
    {
#warning        Now we have to add a entityVector, and access it by the value in the entityIntArr and check if it is an enemy etc. Shouldn't this be handled by something other than the Entity itself? Possible Turn Executor.
        /* There is an entity */
//        intVec[this->x][this->y] = -1;
        Entity* ep = entityVector.at(spaceArrayIndex);
        
        std::cout << "CAN BLOCK?: " << ep->tile->canBlock << std::endl;
        
        if (ep->actorComponent != nullptr && (moveX != 0 || moveY != 0))
        {
            // entity being an actor
            if (ep->tile->canBlock)
            {
                // combat should return entity pointer.
                std::cout << "Entity at x: " << ep->getX() << " and at y: " << ep->getY() << " dies." << std::endl;
                
            } else
            {
                // move
                intVec[this->x][this->y] = -1;
                
                moveX *= C_TILE_IN_GAME_SIZE;
                moveY *= C_TILE_IN_GAME_SIZE;
                    
                this->tile->move(moveX, moveY);
                this->x = this->tile->getPosition().x / C_TILE_IN_GAME_SIZE;
                this->y = this->tile->getPosition().y / C_TILE_IN_GAME_SIZE;
                
                intVec[this->x][this->y] = this->entityVectorPos;
                
            }
        } else
        {

            
        }
    }

}

void Entity::setPosition(int _x, int _y)
{
    this->x = _x;
    this->y = _y;
    
    this->tile->setPosition(_x * C_TILE_IN_GAME_SIZE, _y * C_TILE_IN_GAME_SIZE);
}

void Entity::die(void)
{
    delete this;
}

Entity::~Entity()
{
    delete this->tile;
    
    if (this->actorComponent != nullptr)
    {
        delete this->actorComponent;
    }
}

Entity* Entity::createNewEntityFromSprite(sf::Sprite entitySprite, bool isInvisible, bool blocks, sf::Color entityColor, int x, int y)
{
    Tile* entityTile = new Tile{isInvisible, blocks, entitySprite, entityColor};
    Entity* entity = new Entity{entityTile, x, y};
    return entity;
}

void Entity::setActorComponent(Actor* acp)
{
    this->actorComponent = acp;
}
