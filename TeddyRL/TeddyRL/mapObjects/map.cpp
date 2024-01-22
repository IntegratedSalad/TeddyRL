//
//  map.cpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "map.hpp"
#include "tile.hpp"
#include "entity.hpp"
#include "actor.hpp"

Map::Map()
{
}

Map::Map(const std::vector<sf::Sprite> sv) : spritesVector(sv)
{
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        std::vector<int> vec;
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            vec.push_back(-1);
        }
        blockingEntitiesInt2DVector.push_back(vec);
    }
}

Map::Map(const Map& cm, const std::vector<sf::Sprite> sv) : levelInformationStruct(cm.levelInformationStruct), spritesVector(sv), blockingEntitiesInt2DVector(cm.blockingEntitiesInt2DVector)
{
    const unsigned int toReserve = cm.levelInformationStruct.numOfEntities + 1;
    this->blockingEntities.resize(toReserve);
}

Map::~Map()
{
}

/*
 The difference between PlaceBlockingEntityOnMap and LoadBlockingEntityBackOnMap
 is that besides setting the position (thus in part-initializing the entity), Place...() pushes back
 an entity pointer onto a new empty place at blockingEntities. Load...() doesn't place new pointer,
 it changes the pointer in place.
 */

bool Map::PlaceBlockingEntityOnMap(Entity* entity, int x, int y)
{
    if (!(this->blockingEntitiesInt2DVector[x][y] == -1)) return false;
    this->blockingEntities.push_back(entity);
    entity->blockingEntitiesVectorPos = blockingEntities.size() - 1;
    this->blockingEntitiesInt2DVector[entity->GetX()][entity->GetY()] = entity->blockingEntitiesVectorPos;
    entity->SetPosition(x, y);
    // TODO: There will be a vector of structs
    levelInformationStruct.numOfEntities++;
    return true;
}

void Map::LoadBlockingEntityBackOnMap(Entity* entity)
{
    const unsigned int idx = entity->blockingEntitiesVectorPos;
    this->blockingEntities.at(idx) = entity;
    this->blockingEntitiesInt2DVector[entity->GetX()][entity->GetY()] = entity->blockingEntitiesVectorPos;
}

void Map::RemoveEntityFromMap(Entity* entity)
{
    this->blockingEntitiesInt2DVector[entity->GetX()][entity->GetY()] = -1;
    
    // Do not erase() this vector, because effectively it changes the size, creating problems.
    // Setting to nullptr keeps the size unchanged, but its easy to manage (just check if the entity isn't a nullptr.
    //this->blockingEntities.erase(this->blockingEntities.begin() + entity->blockingEntitiesVectorPos); // erase from entities.
    this->blockingEntities[entity->blockingEntitiesVectorPos] = nullptr;
//    if (levelInformationStruct.numOfEntities > 0)
//       levelInformationStruct.numOfEntities--;
}

void Map::KillEntity(Entity* entity)
{
    sf::Sprite corpseSprite = spritesVector.at(static_cast<int>(TileSprite::CORPSE));
    entity->Die(corpseSprite); // TODO: spawn an item!
    entity = nullptr;
#warning Assert is optimized if optimization is on. Apart from tests, assertion shouldn't be used.
    //assert(entity == nullptr);
}

void Map::drawEnclosingSquare(sf::Sprite wallSprite)
{
    for (int i = 0; i < C_MAP_SIZE; i++)
    {
        for (int j = 0; j < C_MAP_SIZE; j++)
        {
            if (j == 0 || i == 0 || j == C_MAP_SIZE - 1 || i == C_MAP_SIZE - 1)
            {
                Tile* wallTile = new Tile{false, true, wallSprite, sf::Color::White};
#warning As it is with multiple blocks generated, shouldn't they have the same reference? \
         Allocate memory for the Entity object (wall) and place the same object in memory \
         at different locations.
                // TODO: Remove this when we use default constructor for Tile!
                wallTile->SetSpriteEnumVal(TileSprite::BRICK_WALL_1);
                
                Entity* wall = new Entity{wallTile, "Wall", i, j};
                PlaceBlockingEntityOnMap(wall, i, j);
#warning Very important!
                // TODO: Encompass this into a standard method for creating an entity.
            }
        }
    }
}

void Map::GenerateLevel()
{
    // Each wall can have the same pointer to the tile. It won't get destroyed, so why have different pointers?
    
    // Each wall should have a reference to sprite and tile?
    
    std::random_device rnd;
    std::mt19937 rng(rnd());
    
#warning if memory gets bloated by any of this, we will have to redesign tile memory management.
    
    std::uniform_int_distribution<std::mt19937::result_type> rand_pos(1, C_MAP_SIZE - 1);
    std::uniform_int_distribution<std::mt19937::result_type> rand_num(500, 1000);
    
    // TODO: Roll die function
    
    const int randNumOfMonsters = rand_num(rng);
    
    sf::Sprite enemySprite = spritesVector[static_cast<int>(TileSprite::SNAKE)];
    sf::Sprite wallSprite = spritesVector[128];

//    drawEnclosingSquare(wallSprite);
    
    BSPAlgorithm dAlgo = BSPAlgorithm{this, wallSprite}; // provide a list needed sprites
    dAlgo.GenerateLevel(rng);
    
//    for (int i = 0; i < randNumOfMonsters; i++)
//    {
//       Entity* e = Entity::CreateNewEntityFromSprite(enemySprite, "Worm", false, true, sf::Color::White, rand_pos(rng), rand_pos(rng)); // TODO: change this function to have TileSprite as an argument
//        PlaceBlockingEntityOnMap(e, e->GetX(), e->GetY());
//
//        // TODO: Remove this after using default constructor for Tile
//
//        e->tile->SetSpriteEnumVal(TileSprite::SNAKE);
//
//        // TODO: Use new method for setting up AI.
//
//        Actor* acp = new Actor();
//        RandomAI* raip = new RandomAI();
//        acp->SetAI(raip);
//        e->SetActorComponent(acp);
//        acp->SetAIType(AIType::RANDOM);
//    }
}

Entity* Map::GetBlockingEntityPointerFromLocation(int x, int y) const
{
    if ((x > C_MAP_SIZE) || (y > C_MAP_SIZE))
    {
        std::cerr << "Fatal error: accessing entity out of map." << std::endl;
        std::cerr << "X: " << x << " Y: " << y << std::endl;
        exit(-1);
    }
    
    int index = blockingEntitiesInt2DVector[x][y];
    if (index >= 0)
    {
        return blockingEntities[index];
    } else
    {
        return nullptr;
    }
}

Entity* Map::GetBlockingEntityPointerFromEntityVectorPos(int vectorPos) const
{
    return blockingEntities[vectorPos];
}

int Map::GetBlockingEntityIndexFromLocation(int x, int y) const
{
    return blockingEntitiesInt2DVector[x][y];
}

void Map::Clear(void)
{
    std::vector<Entity *>::iterator it;
    auto beginning = this->blockingEntities.begin();
    unsigned int deadEntities = 0;
    unsigned int debugCounter = 0;
    
    std::cout << "blockingEntities size before cleanup: " << this->blockingEntities.size() << std::endl;
    
    for (it = beginning; it != this->blockingEntities.end();) // remember, pointer to end should change!!
    {
        if (*it == nullptr)
        {
            it = this->blockingEntities.erase(it); // return iterator to "the same" position
            deadEntities++;
        } else
        {
            ++it;
        }
    }
    std::cout << "blockingEntities size after cleanup: " << this->blockingEntities.size() << std::endl;
    std::cout << "Dead entities: " << deadEntities << std::endl;
    
    beginning = this->blockingEntities.begin();
    auto end = this->blockingEntities.end();
    for (it = beginning + 1; it != end; it++)
    {
        Entity* ep = *it;
        const Entity* previousEp = *(it - 1);
        const int ex = ep->GetX();
        const int ey = ep->GetY();

        ep->blockingEntitiesVectorPos = previousEp->blockingEntitiesVectorPos + 1;
        this->blockingEntitiesInt2DVector[ex][ey] = ep->blockingEntitiesVectorPos;
        /* I previously made a copy of this vector, and it wasn't getting updated */
    }
    
    // Debug verify, can erase on release
    unsigned int positionInVector = 0;
    for (size_t i = 0; i < this->blockingEntities.size() - 1; i++)
    {
        const Entity* ep1 = this->blockingEntities[i];
        const unsigned int pos1 = ep1->blockingEntitiesVectorPos;
        const Entity* ep2 = this->blockingEntities[i+1];
        const unsigned int pos2 = ep2->blockingEntitiesVectorPos;
        
        if (pos1+1 != pos2)
        {
            std::cout << "Clearing hasn't been done properly" << std::endl;
            std::cout << ep1->blockingEntitiesVectorPos << std::endl;
            std::cout << ep2->blockingEntitiesVectorPos << std::endl;
        }
        if (ep1 == nullptr || ep2 == nullptr)
        {
            std::cout << "Fatal error - nullpointer after clearing map" << std::endl;
            break;
        }
    }
}

DungeonAlgorithm::~DungeonAlgorithm()
{
}

/* Dungeon Algorithm implements these methods, we don't need to get map_p, it's a property of the base class! */

void DungeonAlgorithm::CreateSquareRoom(int x, int y, int w, int h) // creates hollow room surrounded by walls
{
    FillSquareWithWalls(x, y, w, h);
    CarveSquareRoom(x + 1, y + 1, w - 2, h - 2);
}


void DungeonAlgorithm::PlaceWall(int x, int y)
{
    Entity* we = Entity::CreateNewEntityFromSprite(defaultSpriteForWalls, "Wall", false, true, sf::Color::White, x, y); // add tile sprite enum value to this initializer!!!
    
    we->tile->SetSpriteEnumVal(TileSprite::BRICK_WALL_1);
    map_p->PlaceBlockingEntityOnMap(we, x, y);
}


void DungeonAlgorithm::CarveWall(int x, int y)
{
#warning Important!
    // TODO: Check for existing Wall
    Entity* wallToCarve = map_p->GetBlockingEntityPointerFromLocation(x, y);
    // if not nullptr
    if (wallToCarve == nullptr) return;
    map_p->RemoveEntityFromMap(wallToCarve);
    delete wallToCarve;
    wallToCarve = nullptr;
}


void DungeonAlgorithm::CarveSquareRoom(int x, int y, int w, int h)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            CarveWall(x + i, y + j);
        }
    }
}


void DungeonAlgorithm::CarveVerticalLine(int xBegin, int yBegin, int length)
{
    for (int i = 0; i < length; i++)
    {
        CarveWall(xBegin, yBegin + i);
    }
    // TODO: Test
}


void DungeonAlgorithm::CarveHorizontalLine(int xBegin, int yBegin, int length)
{
    for (int i = 0; i < length; i++)
    {
        CarveWall(xBegin, yBegin + i);
    }
    // TODO: Test
}


void DungeonAlgorithm::CarveLine(int xBegin, int yBegin, int xEnd, int yEnd)
{
    // Any line. Bresenham algorithm needed

}

void DungeonAlgorithm::FillMapWithWalls(void)
{
    for (unsigned int x = 0; x < C_MAP_SIZE; x++)
    {
        for (unsigned int y = 0; y < C_MAP_SIZE; y++)
        {
            PlaceWall(x, y);
        }
    }
}

void DungeonAlgorithm::FillSquareWithWalls(int x, int y, int w, int h)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            PlaceWall(x + i, y + j);
        }
    }
}

void BSPAlgorithm::PopulateLevel(std::mt19937& rng, std::unique_ptr<BSPTree> bspTree_p, Room startingRoom)
{
    /* Places player, enemies items, stairs etc... */
    
    LOG_MAP("Checking rootNode in PopulateLevel")
    
    std::shared_ptr<Node> root = bspTree_p->rootNode;
    
    root->childrenNodes[0]->nodeData->prettyPrint();
    root->childrenNodes[1]->nodeData->prettyPrint();
    
    std::cout << "Starting misio room" << std::endl;
    startingRoom.prettyPrint();
    
    
//    exit(0);
    
}

void BSPAlgorithm::BuildLevel(std::mt19937& rng, std::unique_ptr<BSPTree> bspTree_p)
{
    // Build entire level.
    /*
     This involves setting up node data (the max boundary of a room),
     and then setting up actual room data, within confinments of that node.
     After node data is set, room can be carved.
     TODO: Decide upon how will corridors be created.
     
     */
#warning Remember - first half of the level is A, other half is B
    

//    LOG_MAP("A Node:")
//    bspTree_p->rootNode->childrenNodes[0]->nodeData->prettyPrint();
//
//    LOG_MAP("B Node:")
//    bspTree_p->rootNode->childrenNodes[1]->nodeData->prettyPrint();
    
    // 1. Fill map with walls
    // 2. CarveSquareRoom not create Square Room!
    
//    CreateSquareRoom(bspTree_p->rootNode->childrenNodes[0]->nodeData.x,
//                     bspTree_p->rootNode->childrenNodes[0]->nodeData.y,
//                     bspTree_p->rootNode->childrenNodes[0]->nodeData.w,
//                     bspTree_p->rootNode->childrenNodes[0]->nodeData.h); // node A
//
//    CreateSquareRoom(bspTree_p->rootNode->childrenNodes[1]->nodeData.x,
//                     bspTree_p->rootNode->childrenNodes[1]->nodeData.y,
//                     bspTree_p->rootNode->childrenNodes[1]->nodeData.w,
//                     bspTree_p->rootNode->childrenNodes[1]->nodeData.h); // Node B
    
    // ^ two nodes as rooms for now (just to test it)
    
    // Create one room within these two nodes.
    
    // Place misio within one room
    
    
//    return;
    
    std::list<std::shared_ptr<Node>> nodeList;
    Room currentRoom;
    for (int l = 2; l < N_LEVELS_BSP_MAX; l++)
    {
        bspTree_p->GetAllLeavesOnLevel(l, nodeList);
        for (auto leaf : nodeList)
        {
            this->BuildRoom(rng, leaf);
        }
        
        nodeList.clear();
    }
    Room randomRoom = bspTree_p->ChooseRandomRoom(rng);
    PopulateLevel(rng, std::move(bspTree_p), randomRoom);
}

void BSPAlgorithm::BuildRoom(std::mt19937& rng, std::shared_ptr<Node>& node_p)
{
    // 1. Get Node data - x, y, w, h
    // 2. Set random x and y as the starting position of room
    // 3. Set random width and height of this room.
    // 4. CreateSquareRoom
    const int nodeX = node_p->nodeData->x;
    const int nodeY = node_p->nodeData->y;
    const int nodeW = node_p->nodeData->w;
    const int nodeH = node_p->nodeData->h;
    
    if ((nodeW <= 0) || (nodeH <= 0))
    {
        return;
    }
    
    // nodeW or nodeH is smaller than width room_min
    
    int roomWidth = randomNumInRange(WIDTH_ROOM_MIN, WIDTH_ROOM_MAX, rng);
    int roomHeight = randomNumInRange(HEIGHT_ROOM_MIN, HEIGHT_ROOM_MAX, rng);
    
    if (nodeW <= roomWidth)
    {
        roomWidth = nodeW - 4;
    }
    if (nodeH <= roomHeight)
    {
        roomHeight = nodeH - 2;
    }
    
    int roomX = randomNumInRange(nodeX + 1, (nodeX + nodeW) - roomWidth, rng);
    int roomY = randomNumInRange(nodeY + 1, (nodeY + nodeH) - roomHeight, rng);
    
    if (roomX > nodeW)
    {
        roomX = nodeX;
    }
    
    std::cout << "Build room" << std::endl;
    std::cout << "Node X:" << nodeX << " Node Y:" << nodeY << std::endl;
    
    Room* roomData = new Room {.x = roomX, .y = roomY, .w = roomWidth, .h = roomHeight};
    node_p->SetRoomData(roomData);
    CarveSquareRoom(roomX, roomY, roomWidth, roomHeight);
}

/*
 Generate level - entry point method for doing everything that BSPAlgorithm needs.
 
 */
void BSPAlgorithm::GenerateLevel(std::mt19937& rng)
{
    FillMapWithWalls();
    
    // Initiate nodes etc..
    LOG_MAP("Building node tree...")
    std::unique_ptr<BSPTree> nodeTree = BuildNodeTree(rng);
//    LOG_MAP("Printing tree...")
//    nodeTree->PrintTree();
#warning Node's parent is the enclosing node. Parent's children is the division of the parent.
    LOG_MAP("Making rooms")
    BuildLevel(rng, std::move(nodeTree));
    
//    exit(0);
}

std::unique_ptr<BSPTree> BSPAlgorithm::BuildNodeTree(std::mt19937& rng)
{
    std::unique_ptr<BSPTree> bspTree_p(std::make_unique<BSPTree>());
    /* Effectively, BSPAlgorithm will create room data.
     BSPTree and Node are just to form a BSPTree.
     
     */
    /* We can "animate" the tree creation, by later going over the tree in engine, firstly getting the Nodes, and then the actual rooms.
     */
    bspTree_p->treeLeavesNum = bspTree_p->Grow(rng, N_LEVELS_BSP_MAX);
    // TODO: add every leaf to leaves of bspTree_p
    
    return bspTree_p;
}
