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


/* TODO: Maybe we should simplify this?
 
    Instead of having a 2D vector, we should have a 1D vector.
    Then, if we want to move on the (x,y) location, we simply iterate through all entities, and check their position. If on our desired position there isn't anything, we can move. If there is a wall, we stop.
    But what if there are many, many entities and we want to implement AOE?
    Current entityMapVector has the advantage of an instant access at location we want to move.
 */


#warning good idea - implement this.
/*
    TODO: Or another idea: Instead of an actual entity, let us have a 2D vector of ints.
    Each integer is an index, that corresponds with the place of entity in that array.
    Then we simply check, if the location of our desired place to move our entity is greater than -1 (0 being the first index i.e. a possible entity (or even better - a player)).
    If it is, we can then access simply the 1D vector of entities.
 
    Then, when creating an entity, we will take care of assigning him an index in the EntityMapVector2D. Instead of having a 2D array of type Entity, it is an array of ints!
 
 */

typedef std::vector<std::vector<int>> Int2DVec;
typedef std::vector<Int2DVec> LevelsVector;
class Entity;

class Map
{

private:
    
    unsigned int roomsNum;
    int currentLevelNum;
    
    void drawEnclosingSquare(sf::Sprite);
    
    
public:
    
    /* Item vector */
//    EntityMapVector2D itemMapVector; // the same length.
    
    /*  */
    Int2DVec entityIntVec;
    
    LevelsVector levelsVector;
#warning should entityVector be private?
    std::vector<Entity* > entityVector; // TODO: should be private?
    
    Map();
    ~Map();
    
    void removeEntityFromMap(Entity* entity);

//    void pushEntityToEntityVector(Entity*);
    
    void placeEntityOnMap(Entity*, int x, int y);
    
    void generateLevel(const std::vector<sf::Sprite> spritesVector, std::mt19937&);
    
    Entity* getEntityPointerFromLocation(int, int) const;
    Entity* getEntityPointerFromEntityVectorPos(int) const;
    
    int getEntityIndexFromLocation(int, int) const;
    

};

#endif /* map_hpp */
