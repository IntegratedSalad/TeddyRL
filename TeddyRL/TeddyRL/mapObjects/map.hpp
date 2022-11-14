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
#include <random>


/* Because you will be able to go back to the previous levels, every level map will be initialized, and kept in a list. */

typedef std::vector<std::vector<int>> Int2DVec;
typedef std::vector<Int2DVec> LevelsVector; // maybe vector of Map's
class Entity;

class Map
{

private:
    
    unsigned int roomsNum;
    int currentLevelNum;
    
    void drawEnclosingSquare(sf::Sprite);
    
    
public:
    
    /*  */
    Int2DVec blockingEntities2DVector; // Contains blocking entities' position in entityVector (walls, monsters etc).
    // There can be only one entity actor at each place. But there can be multiple items at each place.
    // Used to caclculate paths for AI.
    
    // Items will be in a seperate vector, never blocking sight or movement.
    // If entity dies, its corpse is an item. Can be eaten, can be thrown, can be picked up.
    // If player (or friend) enters this map position, a function is called - 'check for items' that iterates over
    // itemsVector and returns another vector (resized and allocated only to fit the amount of items at that place)
    // So getting items is always iterative - because there can be multiple items on the ground.
    // Large, 2D vecor will be used to alculate paths by getting the 'is blocking' property.
    // There can be Int2DVec for every type of Entity there.
    
    LevelsVector levelsVector;
    std::vector<Entity* > blockingEntities; // contains player
    
    Map();
    ~Map();
    
    void removeEntityFromMap(Entity* entity);

    void placeBlockingEntityOnMap(Entity*, int x, int y);
    
    void generateLevel(const std::vector<sf::Sprite> spritesVector, std::mt19937&);
    
    Entity* getBlockingEntityPointerFromLocation(int, int) const;
    Entity* getBlockingEntityPointerFromEntityVectorPos(int) const;
    
    int getBlockingEntityIndexFromLocation(int, int) const;

};

#endif /* map_hpp */
