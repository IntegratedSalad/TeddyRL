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
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

/* Because you will be able to go back to the previous levels, every level map will be initialized, and kept in a list. */

typedef std::vector<std::vector<int>> Int2DVec;
typedef std::vector<Int2DVec> LevelsVector; // TODO: Struct containing Level info.
class Entity;

class Map
{
private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & roomsNum;
        ar & currentLevelNum;
        ar & blockingEntitiesInt2DVector;
        ar & blockingEntities;
        ar & levelsVector;
    }
    
    unsigned int roomsNum;
    int currentLevelNum;
    
    void drawEnclosingSquare(sf::Sprite);
    
public:
    Int2DVec blockingEntitiesInt2DVector; // Contains blocking entities' position in entityVector (walls, monsters etc)
    std::vector<Entity* > blockingEntities;
    LevelsVector levelsVector;
    
    Map();
    ~Map();
    
    void removeEntityFromMap(Entity* entity);
    void placeBlockingEntityOnMap(Entity*, int x, int y);
    int getBlockingEntityIndexFromLocation(int, int) const;
    
    void generateLevel(const std::vector<sf::Sprite> spritesVector, std::mt19937&);
    
    Entity* getBlockingEntityPointerFromLocation(int, int) const;
    Entity* getBlockingEntityPointerFromEntityVectorPos(int) const;
};

#endif /* map_hpp */
