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
#include <random>

class Map;
class Actor;
class Entity;

/* AI steers actor, and actor gives command to entity. */

class AI
{
private:
    
public:
    
    AI(Actor* a);
    
    virtual ~AI() = 0;
    virtual void make_turn(Map&, Entity* player, std::mt19937& rd) = 0;
    // I don't think that actor pointer should be accessible by other data.
    // For friends, player will be ignored (apart from entities that can help player) and for enemies it will be a target. Later there should be a friend vector.
    
protected:
    Actor* ap;
    
};

class RandomAI : public AI
{
private:
    
public:
    
    RandomAI(Actor* e);
    ~RandomAI();
    
    void make_turn(Map& m, Entity* player, std::mt19937& rd);
    
};

#endif /* ai_hpp */
