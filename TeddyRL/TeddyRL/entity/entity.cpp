//
//  entity.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "entity.hpp"
#include <iostream>

Entity::Entity() : x(0), y(0), actorComponent(nullptr), entityVectorPos(0)
{
    this->setPosition(x, y);
}

Entity::Entity(Tile* _tile, std::string name, int _x, int _y) : x(_x), y(_y), tile(_tile), actorComponent(nullptr), entityVectorPos(0), name(name)
{
    this->setPosition(x, y);
}

Entity::Entity(Tile* _tile, std::string name, int _x, int _y, Actor* comp) : x(_x), y(_y), tile(_tile), actorComponent(comp), entityVectorPos(0), name(name)
{
    this->setPosition(x, y);
}


/* TODO: Later it should return something telling us about attacking etc. */
#warning Maybe we shouldn't use map structures to perform logic on entities but built in methods for distance etc. We should use rects for collision.
// TODO: This method really is making a decision whether to move or not. It should return a turn result.
// TODO: Make an GameAction Map, that binds e.g. 'attack' with an entity or entityVectorPosition.
// TODO: TurnExecutor class.

/* Bumping into something counts as performing action. */
TurnAction Entity::moveOrPerformAction(int moveX, int moveY, Int2DVec& intVec, std::vector<Entity* > entityVector)
{
    int spaceArrayIndex = intVec[this->x + moveX][this->y + moveY]; // TODO: change the name of that variable.
    
    Entity* playerPointer = entityVector[0];
    
    TurnAction actionResult; // TODO: maybe struct is better?
    actionResult.name = "move";
    actionResult.entityPerformingActionVectorPos = this->entityVectorPos;
    
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
        
        /* Setting x and y. */
        this->x = this->tile->getPosition().x / C_TILE_IN_GAME_SIZE;
        this->y = this->tile->getPosition().y / C_TILE_IN_GAME_SIZE;
        
        intVec[this->x][this->y] = this->entityVectorPos;

        // empty action
        return actionResult;
        
    }
    else // return entity and decide what to do with it.
    {
#warning        Now we have to add a entityVector, and access it by the value in the entityIntArr and check if it is an enemy etc. Shouldn't this be handled by something other than the Entity itself? Possible Turn Executor.
        /* There is an entity */
        Entity* ep = entityVector.at(spaceArrayIndex);

        if (ep->tile->canBlock)
        {
            if (ep->actorComponent != nullptr && (moveX != 0 || moveY != 0)) // player attacks entity or entity attacks hostile entity to it.
            {

                actionResult.name = "attack";
                actionResult.entityPerformingActionVectorPos = this->entityVectorPos;
                actionResult.entityTargetOfActionVectorPos = ep->entityVectorPos;
                return actionResult;
                
            } else
            {
                if (ep == playerPointer) // entity attacks player (if entity is not player)
                {
                    std::cout << "MOVEX" << moveX << std::endl;
                    std::cout << "MOVEY" << moveY << std::endl;
                    std::cout << ep << std::endl;
                    std::cout << this << std::endl;
                    std::cout << "SAI: " << spaceArrayIndex << std::endl;
                    
                    bool thisIsPlayer = (this == playerPointer);
                    
                    actionResult.name = "attack";
                    actionResult.entityPerformingActionVectorPos = this->entityVectorPos;
                    actionResult.entityTargetOfActionVectorPos = ep->entityVectorPos;
                    
                    /* Maybe an issue with pointers - some address is being overwritten? */
                    /* BUG: Bumping into wall returns "Teddy attacks Teddy" */
                    
                    return actionResult;
                }
                
                if (ep->actorComponent == nullptr) // door or something else that is bumpable.
                {
                    
                }
                
            }
        }

//        if (ep->actorComponent != nullptr && (moveX != 0 || moveY != 0))
//        {
//            // entity being an actor
//            if (ep->tile->canBlock)
//            {
//                // combat should return entity pointer.
//                std::cout << "Entity at x: " << ep->getX() << " and at y: " << ep->getY() << " dies." << std::endl;
//
//            } else // entity being an actor and being passable.
//            {
//                // move
//                intVec[this->x][this->y] = -1;
//
//                moveX *= C_TILE_IN_GAME_SIZE;
//                moveY *= C_TILE_IN_GAME_SIZE;
//
//                this->tile->move(moveX, moveY);
//                this->x = this->tile->getPosition().x / C_TILE_IN_GAME_SIZE;
//                this->y = this->tile->getPosition().y / C_TILE_IN_GAME_SIZE;
//
//                intVec[this->x][this->y] = this->entityVectorPos;
//
//            }
//
//        } else // passable entity
//        {
//
//        }
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
    // TODO: perform cleanup tasks - call removeEntityFromMap
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

Entity* Entity::createNewEntityFromSprite(sf::Sprite entitySprite, std::string name, bool isInvisible, bool blocks, sf::Color entityColor, int x, int y)
{
    Tile* entityTile = new Tile{isInvisible, blocks, entitySprite, entityColor};
    Entity* entity = new Entity{entityTile, name, x, y};
    return entity;
}

void Entity::setActorComponent(Actor* acp)
{
    this->actorComponent = acp;
}
