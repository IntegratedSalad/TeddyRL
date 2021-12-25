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

class Map
{
private:
    std::vector<std::vector<Entity* >> entityMapVector;
    
public:
    
    Map();
    ~Map();
    
    
};

#endif /* map_hpp */
