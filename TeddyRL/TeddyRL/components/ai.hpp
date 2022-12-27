//
//  ai.hpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef ai_hpp
#define ai_hpp

#include "map.hpp"
#include "actor.hpp"
#include "TurnActions.hpp"
#include <random>

class Map;
class Actor;
class Entity;

class AI
{
private:
public:
    
    AI();
    
    virtual ~AI() = 0;
    virtual ActionResult make_turn(Map&, Entity* player, std::mt19937& rd) = 0;
    // I don't think that actor pointer should be accessible by other data.
    // For friends, player will be ignored (apart from entities that can help player) and for enemies it will be a target. Later there should be a friend vector.
};

class RandomAI : public AI
{
private:
    
public:
    RandomAI();
    ~RandomAI();
    ActionResult make_turn(Map&, Entity* player, std::mt19937& rd); // pass entity
};

#endif /* ai_hpp */
