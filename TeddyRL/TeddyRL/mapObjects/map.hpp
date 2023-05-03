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

enum class LevelType
{
    L_NORMAL = 0,
};

typedef struct LevelInfo
{
    friend class boost::serialization::access;
    unsigned int roomsNum;
    unsigned int numOfEntities;
    unsigned int numOfItems;
    
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & roomsNum;
        ar & numOfEntities;
        ar & numOfItems;
    }
    
} LevelInfo;

typedef std::vector<std::vector<int>> Int2DVec;
typedef std::vector<Int2DVec> LevelsVector; // vector of blockingEntitiesInt2DVector for each level.
// TODO: Struct containing Level info. Or better yet - vector of structs.
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
        
        //ar & blockingEntities; Don't offload this vector. We cannot serialize TileSprite of entity from within entity' serialize() method.
        
        ar & levelsVector;
        ar & levelInformationStruct;
    }
    
    // TODO: Better serialization
    // Maybe Separate loading from saving.
    // Save method: for each of the blocking entity, save entity as it is, and save TileSprite as binary data.
    // We have to have an information how many entities were offloaded on each level.
    // Load method: for each of the blocking entity, get the TileSprite identifier, cast it as TileSprite and load the entity initializing the Tile from the TileSprite
    // entities in blockingEntities reside without Tile.
    // When loading, reserve that many positions in vector, as there are entities.
    // Iterate over some serialized vector and place entities by their blockingEntitiesVectorPos
    
    unsigned int roomsNum;
    int currentLevelNum;
    
    void drawEnclosingSquare(sf::Sprite);
    
    std::vector<sf::Sprite> spritesVector; // canot be const, but whenever accessed, has to be accessed as a const!
    LevelInfo levelInformationStruct; // TODO vector of these.
    
public:
    Int2DVec blockingEntitiesInt2DVector; // Contains blocking entities' position in entityVector (walls, monsters etc)
    std::vector<Entity* > blockingEntities;
    LevelsVector levelsVector;
    
    Map(const std::vector<sf::Sprite> sv);
    Map();
    Map(const Map& cm, const std::vector<sf::Sprite> sv);
    
    ~Map();
    
    void RemoveEntityFromMap(Entity* entity);
    void KillEntity(Entity* entity);
    
    void PlaceBlockingEntityOnMap(Entity*, int x, int y);
    int GetBlockingEntityIndexFromLocation(int, int) const;
    
    void GenerateLevel();
    
    Entity* GetBlockingEntityPointerFromLocation(int, int) const;
    Entity* GetBlockingEntityPointerFromEntityVectorPos(int) const;
    
    unsigned int GetNumberOfEntitiesOfCurrentLevel(void) const
    {
        return levelInformationStruct.numOfEntities;
    }
    
    void SetRoomsNum(unsigned int r) { this->roomsNum = r;}
    void SetSpritesVector(const std::vector<sf::Sprite> sv) {this->spritesVector = sv;}
    
    void SetupLevelInformation(void) {this->levelInformationStruct = LevelInfo{0, 0, 0};}
    
};

#endif /* map_hpp */
