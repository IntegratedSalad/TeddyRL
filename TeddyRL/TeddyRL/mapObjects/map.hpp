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

#define N_LEVELS_BSP_MAX 5
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
    
    unsigned int id = 0;
    
    Node(std::shared_ptr<Node> parent)
    {
        this->parentNode = parent;
        nodeData = nullptr;
        roomData = nullptr;
        this->childrenNodes[0] = nullptr;
        this->childrenNodes[1] = nullptr;
    }
    
    Node(std::shared_ptr<Node> parent, Room* nodeData)
    {
        this->parentNode = parent;
        this->SetNodeData(nodeData);
    }
    
    std::shared_ptr<Node> Left(void)
    {
        return this->childrenNodes[0];
    }
    
    std::shared_ptr<Node> Right(void)
    {
        return this->childrenNodes[1];
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
        this->childrenNodes[0] = nullptr;
        this->childrenNodes[1] = nullptr;
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
    
    // this is looping over every node on each level and creating 2 leaves each time...
    // TODO: find other way to create tree and grow leaves...
    // this doesn't grow leaves as it should...
    void GrowLeavesOnLevel(unsigned int level, unsigned int& nodeId, std::list<std::shared_ptr<Node>>& leavesToGrow) // should be  private
    {
        std::list<std::shared_ptr<Node>> leavesGrownHere;
        for (auto leaf : leavesToGrow)
        {
            std::shared_ptr<Node> firstChildNode = std::make_shared<Node>(leaf); // rename to BNode
            std::shared_ptr<Node> secondChildNode = std::make_shared<Node>(leaf); // rename to ANode
            firstChildNode->id = nodeId;
            nodeId++;
            secondChildNode->id = nodeId;
            nodeId++;
            
            std::cout << "NODE A ID: " << firstChildNode->id << std::endl;
            std::cout << "NODE B ID: " << secondChildNode->id << std::endl;
            
            leaf->childrenNodes[0] = firstChildNode;
            leaf->childrenNodes[1] = secondChildNode;
            leavesGrownHere.push_back(firstChildNode);
            leavesGrownHere.push_back(secondChildNode);
        }
        leavesToGrow.clear();
        leavesToGrow = leavesGrownHere;
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
    // is GetAllLeavesOnLevel wrong????
    
    [[deprecated]]
    bool CheckSymmetry(void)
    {
        std::list<std::shared_ptr<Node>> nodeList;
        for (int i = 0; i < N_LEVELS_BSP_MAX - 1; i++)
        {
            GetAllLeavesOnLevel(i, nodeList);
            for (auto l : nodeList)
            {
                std::cout << l->id << std::endl;
                if ((l->Right() == nullptr) || l->Left() == nullptr)
                {
                    return false;
                }
            }
            nodeList.clear();
        }
        return true;
    }
    
    // This is wrong!!! I didn't test it!!!!
    [[deprecated]]
    void GetAllLeavesOnLevel(const unsigned int level, std::list<std::shared_ptr<Node>>& leaves)
    {
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
    
    void BuildRoomsPreorder(std::shared_ptr<Node> n, std::mt19937& rng, std::list<std::shared_ptr<Node>>& list)
    {
        if (n == nullptr) return;
        list.push_back(n);
        BuildRoomsPreorder(n->Left(), rng, list);
        BuildRoomsPreorder(n->Right(), rng, list);
        
    }
    
    BSPTree(void)
    {
        std::shared_ptr<Node> rootNode_p(std::make_shared<Node>(nullptr));
        rootNode = rootNode_p;
    }
    
    /* Make nodes by splitting the root node */
    // This is wrong I didn't test it!
    unsigned int Grow(std::mt19937& rng, unsigned int levels)
    {
        unsigned int sum;
        unsigned int nodeId = 0;
        int nodeX  =  0;
        int nodeY  =  0;
        int nodeW  =  C_MAP_SIZE;
        int nodeH  =  C_MAP_SIZE;
        RoomType     nodeT  =  RoomType::RT_TREE_NODE;
        std::list<std::shared_ptr<Node>> leavesGrown;

        Room* rootNodeData = new Room{nodeX,
                                      nodeY,
                                      nodeW,
                                      nodeH,
                                      nodeT
        };

        this->rootNode->SetNodeData(rootNodeData); // root encompasses whole level
        this->rootNode->id = nodeId;
        
        nodeId++;
        sum = 1;
        leavesGrown.push_back(this->rootNode);
        for (unsigned int level = 0; level < levels; level++)
        {
            //this->GetAllLeavesOnLevel(level, leavesGrown);
            this->GrowLeavesOnLevel(level + 1, nodeId, leavesGrown);
            sum += std::pow(2, level + 1);
        }
        return sum;
    }
    
    void SplitNodesPreorder(std::shared_ptr<Node> n, std::mt19937& rng)
    {
        // Split nodes and create nodeData
        // Set starting dimensions for root
        if (n == nullptr) return;
        
        int splitX = 0;
        int splitY = 0;
        bool isSplitHorizontal = randomNumInRange(0, 1, rng);
        
        if (n == this->rootNode)
        {
            Room* rootNodeData = new Room{
                .x = 0,
                .y = 0,
                .w = C_MAP_SIZE,
                .h = C_MAP_SIZE,
                .t = RoomType::RT_TREE_NODE
            };
            this->rootNode->SetNodeData(rootNodeData);
        }
        
        Room* nodeAData = new Room{};
        Room* nodeBData = new Room{};
        const Room* pNodeData = n->nodeData; // parent is current node
        
        if (isSplitHorizontal)
        {
            splitY = pNodeData->y + std::floor(pNodeData->h/2);
            splitX = pNodeData->x;
            
            nodeAData->x = pNodeData->x;
            nodeAData->y = pNodeData->y;
            nodeAData->w = pNodeData->w;
            nodeAData->h = std::abs(splitY - pNodeData->y); // shouldn't use abs, has to be non negative
            
            nodeBData->x = pNodeData->x;
            nodeBData->y = splitY + 1;
            nodeBData->w = pNodeData->w;
            nodeBData->h = std::abs(pNodeData->h - nodeAData->h);
        } else
        {
            splitY = pNodeData->y;
            splitX = pNodeData->x + std::floor(pNodeData->w/2);
            
            nodeAData->x = pNodeData->x;
            nodeAData->y = pNodeData->y;
            nodeAData->w = splitX - pNodeData->x;
            nodeAData->h = pNodeData->h;
            
            nodeBData->x = splitX + 1;
            nodeBData->y = pNodeData->y;
            nodeBData->w = pNodeData->w - nodeAData->w;
            nodeBData->h = pNodeData->h;
        }

        if (n->Left() != nullptr)
        {
            n->Left()->SetNodeData(nodeAData);
            n->Left()->nodeData->prettyPrint();
        } else
        {
            std::cout << "left leaf is nullptr" << std::endl;
            delete nodeAData;
        }
        
        if (n->Right() != nullptr)
        {
            n->Right()->SetNodeData(nodeBData);
            n->Right()->nodeData->prettyPrint();
        } else
        {
            delete nodeBData;
        }
        
        SplitNodesPreorder(n->Left(), rng);
        SplitNodesPreorder(n->Right(), rng);
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
        const unsigned int roomsOnThisLevel = (unsigned int)std::pow(2, randomLevel); // there is more
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
