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
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            entityIntArr[i][j] = -1;
        }
    }
}

Map::~Map()
{
    /* Every Entity will delete itself. */
//    for (int i = 0; i < entityVector.size(); i++)
//    {
//        delete entityVector[i];
//    }
    
}

void Map::pushEntityToEntityVector(Entity* e)
{
    // assuming player is at 0
    
    
    
    
}


///* TODO: Here, we erase the air entity - what happens if the entity moves? */
//void Map::insertEntityAtPos(Entity* e, int x, int y)
//{
//    delete this->entityMapVector[x][y]; // delete the air entity.
//    this->entityMapVector[x][y] = e;
//    e->setPosition(x, y);
//}
