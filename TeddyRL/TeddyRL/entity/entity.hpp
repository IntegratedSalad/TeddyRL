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

    ActionResult MoveOrBump(int moveX, int moveY, Int2DVec&, std::vector<Entity* > entityVector);
    ActionResult PickUp(Int2DVec&, std::vector<Entity* >);
    ActionResult OpenDoor(Int2DVec&, std::vector<Entity* >);
    
    void Die(sf::Sprite& corpseSprite);
    // move
    
    bool IsEntityBlocking(void)
    {
        return tile->canBlock;
    }
    
    void SetPosition(int _x, int _y);
    
    void SetX(int _x) { this->x = _x; }
    void SetY(int _y) { this->y = _y; }
    
    int GetX(void) const { return this->x; }
    int GetY(void) const { return this->y; }
    const std::string& GetName(void) const { return this->name; }
    Actor* GetActorComponent(void) const { return this->actorComponent; }
    
    void SetActorComponent(Actor* acp);
    
    void SetTile(Tile* t) { this->tile = t;}
    
    static Entity* CreateNewEntityFromSprite(sf::Sprite entitySprite, std::string name, bool isInvisible, bool blocks, sf::Color entityColor, int x, int y);
    
    unsigned int blockingEntitiesVectorPos;
};


#endif /* entity_hpp */
