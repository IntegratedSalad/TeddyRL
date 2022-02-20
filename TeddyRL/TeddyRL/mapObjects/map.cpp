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
    /* Initialize vector - push air entities */
    
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        std::vector<Entity* > empty_vector;
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            Tile* air_tile = new Tile(true, false);
            Entity* air_entity = new Entity(air_tile, i, j);
            empty_vector.push_back(air_entity);
        }
        
        entityMapVector.push_back(empty_vector);
    }
    
}

Map::~Map()
{
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            delete entityMapVector[i][j];
        }
    }
    
}

/* TODO: Here, we erase the air entity - what happens if the entity moves? */
void Map::insertEntityAtPos(Entity* e, int x, int y)
{
    delete this->entityMapVector[x][y]; // delete the air entity.
    this->entityMapVector[x][y] = e;
    e->setPosition(x, y);
}
