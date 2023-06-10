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

bool Map::PlaceBlockingEntityOnMap(Entity* entity, int x, int y)
{
    if (!(this->blockingEntitiesInt2DVector[x][y] == -1)) return false;
    this->blockingEntities.push_back(entity);
    entity->blockingEntitiesVectorPos = blockingEntities.size() - 1;
    this->blockingEntitiesInt2DVector[entity->GetX()][entity->GetY()] = entity->blockingEntitiesVectorPos;
    entity->SetPosition(x, y);
    // TODO: There will be a vector of structs
    levelInformationStruct.numOfEntities++;
    return true;
}

void Map::LoadBlockingEntityBackOnMap(Entity* entity)
{
//    std::vector<Entity *>::iterator it;
//    it = this->blockingEntities.begin() + entity->blockingEntitiesVectorPos;
//    this->blockingEntities.insert(it, entity);
    this->blockingEntities.push_back(entity); // they go ordered anyway.
}

void Map::RemoveEntityFromMap(Entity* entity)
{
    this->blockingEntitiesInt2DVector[entity->GetX()][entity->GetY()] = -1;
    
    // Do not erase() this vector, because effectively it changes the size, creating problems.
    // Setting to nullptr maybe keeps the size unchanged, but its easy to manage (just check if the entity isn't a nullptr.
    //this->blockingEntities.erase(this->blockingEntities.begin() + entity->blockingEntitiesVectorPos); // erase from entities.
    this->blockingEntities[entity->blockingEntitiesVectorPos] = nullptr;
//    if (levelInformationStruct.numOfEntities > 0)
//       levelInformationStruct.numOfEntities--;
}

void Map::KillEntity(Entity* entity)
{
    sf::Sprite corpseSprite = spritesVector.at(static_cast<int>(TileSprite::CORPSE));
    entity->Die(corpseSprite); // TODO: spawn an item!
    entity = nullptr;
#warning Assert is optimized if optimization is on. Apart from tests, assertion shouldn't be used.
    //assert(entity == nullptr);
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
                PlaceBlockingEntityOnMap(wall, i, j);
            }
        }
    }
}

void Map::GenerateLevel()
{
    // Each wall can have the same pointer to the tile. It won't get destructed, so why have different pointers?
    
    // Each wall should have a reference to sprite and tile?
    
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
#warning if memory gets bloated by any of this, we will have to redesign tile memory management.
    
    std::uniform_int_distribution<std::mt19937::result_type> rand_pos(1, C_MAP_SIZE - 1);
    std::uniform_int_distribution<std::mt19937::result_type> rand_num(300, 500);
    
    const int randNumOfMonsters = rand_num(rng);
    
    sf::Sprite enemySprite = spritesVector[static_cast<int>(TileSprite::SNAKE)];
    sf::Sprite wallSprite = spritesVector[128];

    drawEnclosingSquare(wallSprite);
    
    for (int i = 0; i < randNumOfMonsters; i++)
    {
       Entity* e = Entity::CreateNewEntityFromSprite(enemySprite, "Worm", false, true, sf::Color::White, rand_pos(rng), rand_pos(rng)); // TODO: change this function to have TileSprite as an argument
        PlaceBlockingEntityOnMap(e, e->GetX(), e->GetY());
        
        // TODO: Remove this after using default constructor for Tile
        
        e->tile->SetSpriteEnumVal(TileSprite::SNAKE);
        
        // TODO: Use new method for setting up AI.
        
        Actor* acp = new Actor();
        RandomAI* raip = new RandomAI();
        acp->SetAI(raip);
        e->SetActorComponent(acp);
        acp->SetAIType(AIType::RANDOM);
    }
}

Entity* Map::GetBlockingEntityPointerFromLocation(int x, int y) const
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

Entity* Map::GetBlockingEntityPointerFromEntityVectorPos(int vectorPos) const
{
    return blockingEntities[vectorPos];
}

int Map::GetBlockingEntityIndexFromLocation(int x, int y) const
{
    return blockingEntitiesInt2DVector[x][y];
}

void Map::Clear(void)
{
    std::vector<Entity *>::iterator it;
    auto beginning = this->blockingEntities.begin();
    unsigned int deadEntities = 0;
    unsigned int debugCounter = 0;
    
    std::cout << "blockingEntities size before cleanup: " << this->blockingEntities.size() << std::endl;
    
    for (it = beginning; it != this->blockingEntities.end();) // remember, pointer to end should change!!
    {
        if (*it == nullptr)
        {
            it = this->blockingEntities.erase(it); // return iterator to "the same" position
            deadEntities++;
        } else
        {
            ++it;
        }
    }
    std::cout << "blockingEntities size after cleanup: " << this->blockingEntities.size() << std::endl;
    std::cout << "Dead entities: " << deadEntities << std::endl;
    
    beginning = this->blockingEntities.begin();
    auto end = this->blockingEntities.end();
    for (it = beginning + 1; it != end; it++)
    {
        Entity* ep = *it;
        const Entity* previousEp = *(it - 1);
        const int ex = ep->GetX();
        const int ey = ep->GetY();

        ep->blockingEntitiesVectorPos = previousEp->blockingEntitiesVectorPos + 1;
        this->blockingEntitiesInt2DVector[ex][ey] = ep->blockingEntitiesVectorPos;
        /* I previously made a copy of this vector, and it wasn't getting updated */
    }
    
    // Debug verify, can erase on release
    unsigned int positionInVector = 0;
    for (size_t i = 0; i < this->blockingEntities.size() - 1; i++)
    {
        const Entity* ep1 = this->blockingEntities[i];
        const unsigned int pos1 = ep1->blockingEntitiesVectorPos;
        const Entity* ep2 = this->blockingEntities[i+1];
        const unsigned int pos2 = ep2->blockingEntitiesVectorPos;
        
        if (pos1+1 != pos2)
        {
            std::cout << "Clearing hasn't been done properly" << std::endl;
            std::cout << ep1->blockingEntitiesVectorPos << std::endl;
            std::cout << ep2->blockingEntitiesVectorPos << std::endl;
        }
        if (ep1 == nullptr || ep2 == nullptr)
        {
            std::cout << "Fatal error - nullpointer after clearing map" << std::endl;
            break;
        }
    }
}
