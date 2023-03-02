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

enum class AIType // for serialization
{
    NONE = 0xFF,
    RANDOM = 0xAA,
};

/* TODO: We do not serialize the AI class. Somehow we will have to remember the AI decision after saving.
         This is crucial when we get to the pathfinding - so that AI doesn't calculate new path or new action after loading the save game.
 */

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
    ActionResult make_turn(Map&, Entity* player, std::mt19937& rd);
};

#endif /* ai_hpp */
