//
//  entity.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "entity.hpp"
#include <iostream>

Entity::Entity() : x(0), y(0), actorComponent(nullptr), blockingEntitiesVectorPos(0)
{
    
    /* Do not set position! Setting position involves moving entity's tile, which may not have allocated memory. */
    //this->setPosition(x, y);
}

Entity::Entity(Tile* _tile, std::string name, int _x, int _y) : x(_x), y(_y), tile(_tile), actorComponent(nullptr), blockingEntitiesVectorPos(0), name(name)
{
    this->setPosition(x, y);
}

Entity::Entity(Tile* _tile, std::string name, int _x, int _y, Actor* comp) : x(_x), y(_y), tile(_tile), actorComponent(comp), blockingEntitiesVectorPos(0), name(name)
{
    this->setPosition(x, y);
}

#warning Maybe we shouldn't use map structures to perform logic on entities but built in methods for distance etc. We should use rects for collision.
// TODO: TurnExecutor class.

/* Bumping into something counts as performing action. */
// TODO: pass const map reference, not intvec and entity vector.
ActionResult Entity::moveOrBump(int moveX, int moveY, Int2DVec& intVec, std::vector<Entity* > entityVector)
{
    int indexAtPositionOfMove = intVec[this->x + moveX][this->y + moveY];
    Entity* playerPointer = entityVector[0];
    
    /* This method should be only about moving an entity */
    
    ActionResult moveResult;
    moveResult.type = ActionType::ACTIONTYPE_IDLE;
    moveResult.entityPerformingActionVectorPos = this->blockingEntitiesVectorPos;
    
    if (moveX == 0 && moveY == 0)
    {
        return moveResult;
    }
    
    if (indexAtPositionOfMove < 0)
    {
        /* Tile transformation */
        intVec[this->x][this->y] = -1;
        
        moveX *= C_TILE_IN_GAME_SIZE;
        moveY *= C_TILE_IN_GAME_SIZE;
            
        this->tile->move(moveX, moveY);
        /*                     */
        
        /* Setting x and y. */
        this->x = this->tile->getPosition().x / C_TILE_IN_GAME_SIZE;
        this->y = this->tile->getPosition().y / C_TILE_IN_GAME_SIZE;
        
        intVec[this->x][this->y] = this->blockingEntitiesVectorPos;
        return moveResult;
    }
    else // return entity and decide what to do with it.
    {
        Entity* ep = entityVector.at(indexAtPositionOfMove); // there is an entity
        if (ep != nullptr)
        {
            if (ep->tile->canBlock) // always blocks
            {
                if (moveX != 0 || moveY != 0) // TODO: checked earlier, remove
                {
                    if (ep->actorComponent != nullptr || ep == playerPointer) // player attacks entity or entity attacks player.
                    {
                        // Later, check if the AI is hostile.
                        moveResult.type = ActionType::ACTIONTYPE_ATTACK;
                        moveResult.entityPerformingActionVectorPos = this->blockingEntitiesVectorPos;
                        moveResult.entityTargetOfActionVectorPos = ep->blockingEntitiesVectorPos;
                        return moveResult;
                    }
                }
            }
        }
    }
    return moveResult;
}

void Entity::setPosition(int _x, int _y)
{
    this->x = _x;
    this->y = _y;
    this->tile->setPosition(_x * C_TILE_IN_GAME_SIZE, _y * C_TILE_IN_GAME_SIZE);
}

void Entity::die(sf::Sprite& corpseSprite)
{
    //this->tile->setTile(corpseSprite, sf::Color(100, 100, 100));
    if (this->actorComponent != nullptr)
    {            
        delete this->actorComponent;
        this->actorComponent = nullptr;
        assert(this->actorComponent == nullptr); // TODO: Move this assertions out of the working code.
    }
    delete this;
}

Entity::~Entity()
{
    //this->tile->canBlock = false;
//    delete this->tile;
//    this->tile = nullptr;
//    assert(this->tile == nullptr);
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
