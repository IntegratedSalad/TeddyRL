//
//  entity.hpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef entity_hpp
#define entity_hpp

#include <iostream>
#include <memory>
#include <vector>
#include "tile.hpp"
#include "actor.hpp"
#include "constants.hpp"
#include "TurnActions.hpp"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

/* Entity is a single object on game map, that isn't a free space.
   Entity has a pointer to Tile object, that is its graphical representation.
 */

typedef std::vector<std::vector<int>> Int2DVec;

class Actor;
class Entity
{
private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & actorComponent; // TODO: Serialize actor pointer contents.
        ar & name;
        ar & x;
        ar & y;
        ar & blockingEntitiesVectorPos;
        //ar & tile; TODO: Where to save tile->GetSpriteEnumVal()? Custom Struct that serializes this?
    }
    
    Actor* actorComponent;
    
    int x;
    int y;
    
    std::string name;
    
public:
    Tile* tile;
    
    Entity();
    Entity(Tile* _tile, std::string name, int _x, int _y);
    Entity(Tile* _tile, std::string name, int _x, int _y, Actor*);
    
    Entity(const Entity&); // Not making it const throws - "The specified type does not meet the requirements of Cpp17MoveInsertable"
    
    ~Entity();

    ActionResult moveOrBump(int moveX, int moveY, Int2DVec&, std::vector<Entity* > entityVector);
    ActionResult pickUp(Int2DVec&, std::vector<Entity* >);
    ActionResult openDoor(Int2DVec&, std::vector<Entity* >);
    
    void die(sf::Sprite& corpseSprite);
    // move
    
    bool isEntityBlocking(void)
    {
        return tile->canBlock;
    }
    
    void setPosition(int _x, int _y);
    
    void setX(int _x) { this->x = _x; }
    void setY(int _y) { this->y = _y; }
    
    int getX(void) const { return this->x; }
    int getY(void) const { return this->y; }
    const std::string& getName(void) const { return this->name; }
    Actor* getActorComponent(void) const { return this->actorComponent; }
    
    void setActorComponent(Actor* acp);
    
    void SetTile(Tile* t) { this->tile = t;}
    
    static Entity* createNewEntityFromSprite(sf::Sprite entitySprite, std::string name, bool isInvisible, bool blocks, sf::Color entityColor, int x, int y);
    
    unsigned int blockingEntitiesVectorPos;
};


#endif /* entity_hpp */
