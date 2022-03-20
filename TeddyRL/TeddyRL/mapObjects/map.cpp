//
//  map.cpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "map.hpp"
#include "tile.hpp"


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

void Map::placeEntityOnMap(Entity* entity, int x, int y)
{
    this->entityVector.push_back(entity);
    entity->entityVectorPos = entityVector.size() - 1;
    this->entityIntVec[entity->getX()][entity->getY()] = entity->entityVectorPos;
}
