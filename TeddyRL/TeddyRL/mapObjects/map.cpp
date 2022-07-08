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


Map::Map()
{
    
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        std::vector<int> vec;
        
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            vec.push_back(-1);
        }
        entityIntVec.push_back(vec);
    }
}

Map::~Map()
{
    /* ! Every Entity will delete itself so we don't delete anything in entityVector */

}

void Map::pushEntityToEntityVector(Entity* e)
{
    /* Assuming player is at 0 */
    

    
}

// TODO: Make this return a boolean, depending if there is something at entityIntVec[x][y].
void Map::placeEntityOnMap(Entity* entity, int x, int y)
{
    this->entityVector.push_back(entity);
    entity->entityVectorPos = entityVector.size() - 1;
    this->entityIntVec[entity->getX()][entity->getY()] = entity->entityVectorPos;
    entity->setPosition(x, y);
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
                Entity* wall = new Entity{wallTile, i, j};
                placeEntityOnMap(wall, i, j);
                
            }
        }
    }
}

void Map::generateLevel(const std::vector<sf::Sprite> spritesVector)
{
    // Each wall can have the same pointer to the tile. It won't get destructed, so why have different pointers?
    
    // Each wall should have a reference to sprite and tile?
#warning if memory gets bloated by any of this, we will have to redesign tile memory management.
    
    sf::Sprite wallSprite = spritesVector[128];

    drawEnclosingSquare(wallSprite);
    
}
