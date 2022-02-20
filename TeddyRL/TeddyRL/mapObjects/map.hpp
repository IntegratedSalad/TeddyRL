//
//  map.hpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <iostream>
#include "entity.hpp"
#include "constants.hpp"

/* Because you will be able to go back to the previous levels, every level map will be initialized, and kept in a list. */
typedef std::vector<std::vector<Entity* >> EntityMapVector2D;
struct Map
{

public:
    
    EntityMapVector2D entityMapVector; /* This shouldn't really be implemented like that. It means that we have a 2D vector of pointers to the Entity with all the sprites etc. But, because it will be a simple roguelike, I'm not going to do anything that would differ from a sample, tutorial roguelike. */
    
    EntityMapVector2D itemMapVector; // the same length.
    
    /* Item vector */
    
    int levelNum;
    
    Map();
    ~Map();
    
    void removeEntityFromMap();
    void generateLevel();
    
    void insertEntityAtPos(Entity* e, int x, int y);
    

};

#endif /* map_hpp */
