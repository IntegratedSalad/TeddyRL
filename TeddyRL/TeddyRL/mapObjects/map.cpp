//
//  map.cpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "map.hpp"
#include "tile.hpp"
#include "entity.hpp"
#include "actor.hpp"

Map::Map()
{
}

Map::Map(const std::vector<sf::Sprite> sv) : spritesVector(sv)
{
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        std::vector<int> vec;
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            vec.push_back(-1);
        }
        blockingEntitiesInt2DVector.push_back(vec);
    }
}

Map::Map(const Map& cm, const std::vector<sf::Sprite> sv) : levelInformationStruct(cm.levelInformationStruct), spritesVector(sv), blockingEntitiesInt2DVector(cm.blockingEntitiesInt2DVector)
{
}

Map::~Map()
{
}

// TODO: Make this return a boolean, depending if there is something at entityIntVec[x][y].
void Map::placeBlockingEntityOnMap(Entity* entity, int x, int y)
{
    this->blockingEntities.push_back(entity);
    entity->blockingEntitiesVectorPos = blockingEntities.size() - 1;
    this->blockingEntitiesInt2DVector[entity->getX()][entity->getY()] = entity->blockingEntitiesVectorPos;
    entity->setPosition(x, y); // TODO: here the bad memory access happens
    
    // TODO: increase current level's numOfEntities | There will be a vector of structs
    
    levelInformationStruct.numOfEntities++;
}

void Map::removeEntityFromMap(Entity* entity)
{
    //delete this->blockingEntities[entity->actorsVectorPos];
    this->blockingEntitiesInt2DVector[entity->getX()][entity->getY()] = -1;
    this->blockingEntities.erase(this->blockingEntities.begin() + entity->blockingEntitiesVectorPos); // erase from entities.
    
    if (levelInformationStruct.numOfEntities > 0)
        levelInformationStruct.numOfEntities--;
}

void Map::KillEntity(Entity* entity)
{
    sf::Sprite corpseSprite = spritesVector.at(static_cast<int>(TileSprite::CORPSE));
    entity->die(corpseSprite); // TODO: spawn an item!
    entity = nullptr;
#warning Assert is optimized if optimization is on. Apart from tests, assertion shouldn't be used.
    assert(entity == nullptr);
}

void Map::drawEnclosingSquare(sf::Sprite wallSprite)
{
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            if (j == 0 || i == 0 || j == C_MAP_SIZE - 1 || i == C_MAP_SIZE - 1)
            {
                Tile* wallTile = new Tile{false, true, wallSprite, sf::Color::White};
                
                // TODO: Remove this when we use default constructor for Tile!
                wallTile->SetSpriteEnumVal(TileSprite::BRICK_WALL_1);
                
                Entity* wall = new Entity{wallTile, "Wall", i, j};
                placeBlockingEntityOnMap(wall, i, j);
            }
        }
    }
}

void Map::generateLevel()
{
    // Each wall can have the same pointer to the tile. It won't get destructed, so why have different pointers?
    
    // Each wall should have a reference to sprite and tile?
    
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
#warning if memory gets bloated by any of this, we will have to redesign tile memory management.
    
    std::uniform_int_distribution<std::mt19937::result_type> rand_pos(1, 38);
    std::uniform_int_distribution<std::mt19937::result_type> rand_num(1, 1);
    
    const int randNumOfMonsters = rand_num(rng);
    
    sf::Sprite enemySprite = spritesVector[static_cast<int>(TileSprite::SNAKE)];
    sf::Sprite wallSprite = spritesVector[128];

    drawEnclosingSquare(wallSprite);
    
    for (int i = 0; i < randNumOfMonsters; i++)
    {
       Entity* e = Entity::createNewEntityFromSprite(enemySprite, "Worm", false, true, sf::Color::White, rand_pos(rng), rand_pos(rng)); // TODO: change this function to have TileSprite as an argument
        placeBlockingEntityOnMap(e, e->getX(), e->getY());
        
        // TODO: Remove this after using default constructor for Tile
        
        e->tile->SetSpriteEnumVal(TileSprite::SNAKE);
        
        // TODO: Use new method for setting up AI.
        
        Actor* acp = new Actor();
        RandomAI* raip = new RandomAI();
        acp->setAI(raip);
        e->setActorComponent(acp);
        acp->setAIType(AIType::RANDOM);
    }
}

Entity* Map::getBlockingEntityPointerFromLocation(int x, int y) const
{
    int index = blockingEntitiesInt2DVector[x][y];
    if (index >= 0)
    {
        return blockingEntities[index];
    } else
    {
        return nullptr;
    }
}

Entity* Map::getBlockingEntityPointerFromEntityVectorPos(int vectorPos) const
{
    return blockingEntities[vectorPos];
}

int Map::getBlockingEntityIndexFromLocation(int x, int y) const
{
    return blockingEntitiesInt2DVector[x][y];
}

