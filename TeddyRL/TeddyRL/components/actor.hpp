//
//  actor.hpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef actor_hpp
#define actor_hpp
#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "ai.hpp"
#include "entity.hpp"
#include "turnAction.hpp"
#include <random>

/* Actor is a component that relies on AI that it's given */
typedef std::vector<std::vector<int>> Int2DVec;
class Entity;
class Map;

class AI;

/* Maybe actor should subclass Entity? */
class Actor
{
private:
    
    int hp;
    int mana;
    AI* ai;

public:
    
    Actor(Entity*);
    ~Actor();
    Entity* ownEntity;
#warning we should think about what component has what
    TurnAction make_turn(Int2DVec& intVec, std::vector<Entity* > entityVector, Map& m, std::mt19937 &rd, Entity* player);
    
    void setAI(AI* aip) {this->ai = aip;}

    
};

#endif /* actor_hpp */
