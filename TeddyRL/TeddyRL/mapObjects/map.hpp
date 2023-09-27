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

/* Because you will be able to go back to the previous levels, every level map will be initialized, and kept in a vector. */

enum class LevelType
{
    L_NORMAL = 0,
    L_FIRST, // give player some equipment and a good monster to tame using amulet.
    L_CAVE, // cellular automata
    L_END
};

/* Will be used for e.g. AI - where to flee etc. */
/* Set by algorithm, that knows when best to place them */
enum class RoomType
{
    RT_DEFAULT = 0,
    RT_LAIR,
    RT_BOSS,
    RT_ARTIFACT,
    RT_BRANCH,
    
    RT_TREE_NODE,
    RT_TREE_CORRIDOR
};

/* Although a square, e.g. in Cellular Automata it won't be a room, just a "zone" with properties. */
typedef struct Room
{
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    RoomType     t;
    
} Room;
/* Add data from Algorithm (creation of levels):
   Where are rooms, and what is their dimension (we can then spawn enemies in them).
   Map spawns enemies and items decides whether to put an artifact, boss etc.
 */

// TODO: Maybe add Room array to keep information about rooms.
typedef struct LevelInfo
{
    friend class boost::serialization::access;
    unsigned int roomsNum;
    unsigned int numOfEntities;
    unsigned int numOfItems;
    
    // Possible need of expanding this structure to hold rooms info.
    
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
// TODO: When changing levels, destroy Tiles of entities of previous level!
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
    
    bool PlaceBlockingEntityOnMap(Entity*, int x, int y);
    void LoadBlockingEntityBackOnMap(Entity*);
    int GetBlockingEntityIndexFromLocation(int, int) const;
    
    void GenerateLevel();
    void Clear(void);
    
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

/* Utils for dungeon generation */

typedef struct Node
{
    Node* parentNode;
    Node* childrenNodes[2];
    Room nodeData;
    Room roomData;
} Node;

typedef struct Tree
{
    Node* rootNode;
    std::vector<Node*> leaves;
} Tree;


class DungeonAlgorithm
{
private:
    sf::Sprite defaultSpriteForWalls;
    Map* map_p;
    
protected:
    Map* getMapPointer(void) {return map_p;}
    sf::Sprite getWallSprite(void) {return defaultSpriteForWalls;}
    void CreateSquareRoom(int x, int y, int w, int h);
    void CarveSquareRoom(int x, int y, int w, int h);
    void PlaceWall(int x, int y);
    void CarveWall(int x, int y);
    void CarveVerticalLine(int xBegin, int yBegin, int length);
    void CarveHorizontalLine(int xBegin, int yBegin, int length);
    void CarveLine(int xBegin, int yBegin, int xEnd, int yEnd);
    void FillMapWithWalls(void);
    virtual void GenerateLevel(std::mt19937&) = 0;
    
public:
    DungeonAlgorithm(Map* mp, sf::Sprite wSprite) : map_p(mp), defaultSpriteForWalls(wSprite)
    {
    }
    
    virtual ~DungeonAlgorithm() = 0;
};


/*
 BSP Algorithm.
 
 1. Initialize Tree data.
 2. Make walls.


 TODO: Question:
  What data should the Map have after generating dungeon with an algorithm?
  Will number of rooms, position of rooms and their types be needed?
  For now, let us just implement the algorithm.
  NodeTree will be used to build map and then the "metadata" -
  - the data that arose from building a tree - room positions and sizes, room types can be copied into LevelInfo
*/

class BSPAlgorithm : private DungeonAlgorithm
{
private:
    unsigned int rooms;
    
public:
    BSPAlgorithm(Map* map_p, sf::Sprite wSprite) : DungeonAlgorithm(map_p, wSprite)
    {
    }
    void GenerateLevel(std::mt19937&);
    Tree BuildNodeTree(std::mt19937&);
    void BuildLevel(std::mt19937&);
    void PopulateLevel(const Tree* const, std::mt19937&);
//    Tree*
};


class CellularAutomataAlgorithm : private DungeonAlgorithm
{
};

#endif /* map_hpp */
