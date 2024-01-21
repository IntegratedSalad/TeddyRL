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
#include "utils.hpp"
#include <random>
#include <memory>
#include <list>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#define N_LEVELS_BSP_MAX 5 //5
#define WIDTH_ROOM_MIN 5
#define HEIGHT_ROOM_MIN 5
#define WIDTH_ROOM_MAX 7
#define HEIGHT_ROOM_MAX 7

#define LOG_MAP(x) std::cout << "LOG_MAP:\n " << (x) << std::endl;
// TODO: Build that macro, so I can add variadic arguments and print more than one thing

// TODO: Move LOG utilities to utils.h

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
    int      x;
    int      y;
    int      w;
    int      h;
    RoomType t;
    
    bool Intersects(const Room& other)
    {
        // 1. Decide position
        // 2. Decide if overlaps
        
        if (this->x >= other.x &&
            this->y >= other.y)
        {
            if (other.w + other.x >= this->x &&
                other.y + other.h >= this->y)
            {
                return true;
            }
        } else if (this->x >= other.x &&
                   this->y <= other.y)
        {
            if (other.x + other.w >= this->y + this->h &&
                this->y + this->h >= other.y)
            {
                return true;
            }
        } else if (this->x <= other.x &&
                   this->y <= other.y)
        {
            if ((this->x + this->w) >= other.x
                && (this->y + this->h) >= other.y)
            {
                return true;
            }
        } else if (this->x <= other.x &&
                   this->y >= other.y)
        {
            if (this->x + this->w >= other.y + other.h)
            {
                return true;
            }
        }
        return false;
    }
    
    void prettyPrint(void)
    {
        std::cout << "Room is W:" << w << " by: H " << h << std::endl;
        std::cout << "X: " << x << " Y: " << y << std::endl;
    }
    
} Room;
/* Add data from Algorithm (creation of levels):
   Where are rooms, and what is their dimension (we can then spawn enemies in them).
   Map spawns enemies and items decides whether to put an artifact, boss etc.
 */

// TODO: Maybe add Room array to keep information about rooms.
// TODO: Exactly, a uniform Room data collection regardless of used algorithm.
//       Where are stares? Where can be/is treasure spawned etc.
typedef struct LevelInfo
{
    friend class boost::serialization::access;
    unsigned int roomsNum;
    unsigned int numOfEntities;
    unsigned int numOfItems;
    
    // Possible need of expanding this structure to hold rooms info.
    // Rooms number is just leaves vector size from BSPTree.
    
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
    std::shared_ptr<Node> parentNode;
    std::shared_ptr<Node> childrenNodes[2];
    
    Room* nodeData;
    Room* roomData;
    
    Node(std::shared_ptr<Node> parent)
    {
        this->parentNode = parent;
        nodeData = nullptr;
        roomData = nullptr;
    }
    
    Node(std::shared_ptr<Node> parent, Room* nodeData)
    {
        this->parentNode = parent;
        this->SetNodeData(nodeData);
    }
    
    void SetNodeData(Room* data)
    {
        this->nodeData = data;
    }
    
    void SetRoomData(Room* data)
    {
        this->roomData = data;
    }
    
    Node()
    {
        nodeData = nullptr;
        roomData = nullptr;
    }
    ~Node() {delete nodeData; delete roomData;};
    
} Node;

typedef struct BSPTree
{
    std::shared_ptr<Node> rootNode;
    std::vector<std::shared_ptr<Node>> leaves; // not needed
    std::vector<std::unique_ptr<Node>> bottomLeaves; // not needed
    unsigned int treeLeavesNum = 0;
    
    std::shared_ptr<Node> GetSisterNode(std::shared_ptr<Node> node)
    {
        std::shared_ptr<Node> sister;
        std::shared_ptr<Node> parent = node->parentNode; // go to parent
        sister = parent->childrenNodes[0]; // assign 1st element
        if (sister == node) { sister = parent->childrenNodes[1]; } // check if first element is this node. If yes, sister is the second node.
        return sister;
    }
    
    unsigned int GrowLeavesOnLevel(std::mt19937& rng, unsigned int level, std::list<std::shared_ptr<Node>>& previouslyGrownLeaves, bool& quit) // should be  private
    {
        unsigned int leavesOnThisLevel = (unsigned int)std::pow(2, level);
        unsigned int createdLeaves = 0;
        std::list<std::shared_ptr<Node>> leavesToGrow; // leaves that will be grown now.
//        std::shared<
        //int i = 0;
        
        bool splitDirIsHorizontal = 1; // 0: Vertical, 1: Horizontal
        
        Room* parentNodeData;
        Room* nodeAData = new Room{};
        Room* nodeBData = new Room{};
        
        nodeAData->t = RoomType::RT_TREE_NODE;
        nodeBData->t = RoomType::RT_TREE_NODE;
        int offset;
        int splitStartX = 0;
        int splitStartY = 0;
        for (auto leaf : previouslyGrownLeaves)
        {
            // How to split:
            // Take starting y and x of a node and its height and width.
            // if splitDirIsHorizontal:
            // take random offset from middle y of the node
            // else: take random offset from middle x of the node.
            // Draw wall line.
            // Take note of its position.
            // node A will be leftmost/uppermost depending on splitDir
            // because this split always cuts the parent Node into two other nodes.
            parentNodeData = leaf->nodeData;
            offset = rollDie(DieToThrow::D8, rng);
            std::cout << "Parent room:" <<  std::endl;
            parentNodeData->prettyPrint();
            if (splitDirIsHorizontal)
            {
                std::cout << "Horizontal split" << std::endl;
                // We know that the line will not be wider than the width of a node
                splitStartY = parentNodeData->y + std::floor(parentNodeData->h / 2) + offset;
                splitStartX = parentNodeData->x;
                
                nodeAData->x = parentNodeData->x;
                nodeAData->y = parentNodeData->y;
                nodeAData->w = parentNodeData->w;
                nodeAData->h = parentNodeData->h - splitStartY;
                
                nodeBData->x = nodeAData->x;
                nodeBData->y = nodeAData->h + 2; // at + 1 is wall
                nodeBData->w = nodeAData->w;
                
                if ((parentNodeData->h - nodeAData->h - 1) < 0 ||
                    (nodeAData->h < 0))
                {
                    quit = true;
                    return 0;
                } else
                {
                    nodeBData->h = (parentNodeData->h - nodeAData->h - 1); // -1 is for wall
                }
            } else // vertical split
            {
                std::cout << "Vertical split" << std::endl;
                splitStartY = parentNodeData->y;
                splitStartX = parentNodeData->x + std::floor(parentNodeData->w / 2) + offset;
                
                nodeAData->x = parentNodeData->x;
                nodeAData->y = parentNodeData->y;
                nodeAData->w = parentNodeData->x + parentNodeData->w - splitStartX;
                nodeAData->h = parentNodeData->h;
                
                nodeBData->x = nodeAData->w + 2;
                nodeBData->y = nodeAData->y;
                nodeBData->h = nodeAData->h;
                if ((parentNodeData->w - nodeAData->w - 1) < 0 ||
                    (nodeAData->w < 0))
                {
                    quit = true;
                    return 0;
                } else
                {
                    nodeBData->w = parentNodeData->w - nodeAData->w - 1;
                }
            }
            
            std::cout << "Room A:" <<  std::endl;
            nodeAData->prettyPrint();
            std::cout << "Room B:" <<  std::endl;
            nodeBData->prettyPrint();
            
            std::shared_ptr<Node> firstChildNode = std::make_shared<Node>(leaf, nodeAData); // rename to BNode
            std::shared_ptr<Node> secondChildNode = std::make_shared<Node>(leaf, nodeBData); // rename to ANode
            leaf->childrenNodes[0] = firstChildNode;
            leaf->childrenNodes[1] = secondChildNode;
            /*  */
            leavesToGrow.push_back(firstChildNode);
            leavesToGrow.push_back(secondChildNode);
            createdLeaves++;
            splitDirIsHorizontal = !splitDirIsHorizontal;
        }
        
        previouslyGrownLeaves.clear();
        std::copy(leavesToGrow.begin(), leavesToGrow.end(), std::back_inserter(previouslyGrownLeaves));
        previouslyGrownLeaves = leavesToGrow;
        return createdLeaves;
    }
    
    // Maybe generate combinations?
    
    // For example:
    /*
     Getting all 2nd level nodes:
     Combinations num: 2 -> [0 0] [0 1] [1 0] [1 1]
     
     Getting all 3rd level nodes:
     Combinations num: 3 -> [0 0 0] [0 0 1] [0 1 0] [0 1 1]
                            [1 0 0] [1 0 1] [1 1 0] [1 1 1]
     
     */
    
    void GetAllLeavesOnLevel(const unsigned int level, std::list<std::shared_ptr<Node>>& leaves)
    {
        // TODO: This could be a recursive function
//        std::list<std::shared_ptr<Node>> list;
        const unsigned int totalLeavesOnLevel = std::pow(2, level);
        if (level <= 0)
        {
            leaves.push_back(this->rootNode);
            return;
        }
        else if (level == 1)
        {
            leaves.push_back(this->rootNode->childrenNodes[0]);
            leaves.push_back(this->rootNode->childrenNodes[1]);
            return;
        }
        else
        {
            int combinationsBitField = 0; // 32 bit
            std::shared_ptr<Node> currentNode = this->rootNode;
            const unsigned int totalCombinations = std::pow(2, level);
            for (int i = 0; i < totalCombinations; i++)
            {
                for (int j = 0; j < level; j++)
                {
                    const bool index = (combinationsBitField << ((level -1) - j) & 1) == 1;
//                    currentNode = currentNode->childrenNodes[
//                    ((combinationsBitField << ((level - 1) - j)) & 1) == 1 ];
                    // This will "scroll" bits in the combinationBitField from left to right accessing the combination of childrenNodes.
                    
                    currentNode = currentNode->childrenNodes[index];
                }
                
                leaves.push_back(currentNode);
                combinationsBitField++;
                currentNode = this->rootNode;
            }
        }
        assert(leaves.size() == totalLeavesOnLevel);
    }
    
//    [[nodiscard]] std::shared_ptr<Node> GetCousin(std::shared_ptr<Node> node, unsigned int level)
//    {
//        /* Cousin is a node that has the same grandparent but not the same parent. */
//        /* Method will return first node in the childrenNodes array */
//        /* Level 0 has 0 cousins (of a one node)
//           Level 1 has 0 cousins (of a one node)
//           Level 2 has 2 cousins (of a one node)
//           Level 3 has 3 cousins (of a one node)
//           ...
//         */
//        std::shared_ptr<Node> currentNode;
//    }
    
    BSPTree(void)
    {
        std::shared_ptr<Node> rootNode_p(std::make_shared<Node>(nullptr));
        rootNode = rootNode_p;
    }
    
    /* Make tree and return num of leaves. */
    unsigned int Grow(std::mt19937& rng, unsigned int levels)
    {
        // We only set Node data here. No drawing, no room setting
        
        int nodeX  =  0;
        int nodeY  =  0;
        int nodeW  =  C_MAP_SIZE;
        int nodeH  =  C_MAP_SIZE;
        RoomType     nodeT  =  RoomType::RT_TREE_NODE;
        
        Room* rootNodeData = new Room{nodeX,
                                      nodeY,
                                      nodeW,
                                      nodeH,
                                      nodeT
        };
        this->rootNode->SetNodeData(rootNodeData); // root encompasses whole level
        
        unsigned int nodeOffset = rollDie(DieToThrow::D8, rng);
        
        std::shared_ptr<Node> currentNode = this->rootNode;
        std::list<std::shared_ptr<Node>> leavesGrown;
        currentNode->childrenNodes[0] = std::make_shared<Node>(currentNode);
        currentNode->childrenNodes[1] = std::make_shared<Node>(currentNode);
        
        Room* firstANodeData = new Room{};
        firstANodeData->w = (rootNodeData->w / 2); // + nodeOffset
        firstANodeData->x = rootNodeData->x;
        firstANodeData->h = rootNodeData->h;
        firstANodeData->y = rootNodeData->y;
        firstANodeData->t = RoomType::RT_TREE_NODE;
        currentNode->childrenNodes[0]->SetNodeData(firstANodeData);
        
        Room* firstBNodeData = new Room{};
        firstBNodeData->w = C_MAP_SIZE - firstANodeData->w;
        firstBNodeData->x = firstBNodeData->w - 1;
        firstBNodeData->y = firstANodeData->y;
        firstBNodeData->h = firstANodeData->h;
        firstBNodeData->t = RoomType::RT_TREE_NODE;
        currentNode->childrenNodes[1]->SetNodeData(firstBNodeData);
        
        leavesGrown.push_back(currentNode->childrenNodes[0]);
        leavesGrown.push_back(currentNode->childrenNodes[1]);
        this->leaves.push_back(currentNode->childrenNodes[0]);
        this->leaves.push_back(currentNode->childrenNodes[1]);
        
        // Make list of previously created leaves. We won't be needing to traverse
        // the tree from root, but keep track of previously created leaves.
        
        unsigned int leavesCreated = 0;
        bool splitDirIsHorizontal = 1;
        bool quit = false;
        for (unsigned int level = 1; level < levels; level++)
        {
            leavesCreated += GrowLeavesOnLevel(rng, level, leavesGrown, quit);
            if (quit) break;
        }
        return leavesCreated + 2; // we've manually created two
    }
    
    void GetSubtreeBeneathNode(std::shared_ptr<Node> n, std::list<std::shared_ptr<Node>>& leaves)
    {
        // Recursively?
    }
    
    Room MakeSubnode(std::mt19937& rng, std::shared_ptr<Node> parentNode)
    {
        Room subnode;
        const Room* parentNodeData = parentNode->nodeData;
        // We need sister node?
        // Or, divide the node in half + offset and make two subnodes...
        
        return subnode;
    }
    
    Room ChooseRandomRoom(std::mt19937& rng)
    {
        // while not nullptr
        const unsigned int randomLevel = randomNumInRange(2, N_LEVELS_BSP_MAX - 1, rng);
        const unsigned int roomsOnThisLevel = (unsigned int)std::pow(2, randomLevel);
        const unsigned int randomRoom = randomNumInRange(0, roomsOnThisLevel, rng);
        std::list<std::shared_ptr<Node>> nodesList;
        GetAllLeavesOnLevel(randomLevel, nodesList);
        Room roomChosen;
        int i = 0;
        for (auto node : nodesList)
        {
            roomChosen = *node->roomData;
            if (i == randomRoom) break;
            i++;
        }
        return roomChosen;
    }
    
    // TODO: More methods to traverse BSPTree in order to generate leaves
    // TODO: Traverse method, that will return pointer iteratively to each leaf
    //       This will be used to generate room data.
    
} BSPTree;


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
    void FillSquareWithWalls(int x, int y, int w, int h);
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
    std::unique_ptr<BSPTree> BuildNodeTree(std::mt19937&);
    void BuildLevel(std::mt19937& rng, std::unique_ptr<BSPTree> bspTree_p); // TODO: should be virtual void
    void BuildRoom(std::mt19937& rng, std::shared_ptr<Node>& node_p);
    void PopulateLevel(std::mt19937&, std::unique_ptr<BSPTree> bspTree_p, Room startingRoom); // TODO: should be virtual void
//    Tree*
};


class CellularAutomataAlgorithm : private DungeonAlgorithm
{
};

#endif /* map_hpp */
