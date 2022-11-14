//
//  actor.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "actor.hpp"

Actor::Actor(Entity* ep) : ownEntity(ep)
{
    
}

Actor::~Actor()
{
   delete this->ai;
}

TurnAction Actor::make_turn(Int2DVec& intVec, std::vector<Entity* > entityVector, Map& m, std::mt19937 &rd, Entity* player)
{
    TurnAction turnResult = this->ai->make_turn(m, player, rd);
    return turnResult;
}
