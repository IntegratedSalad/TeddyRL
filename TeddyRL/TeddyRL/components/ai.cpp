//
//  ai.cpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "ai.hpp"

AI::AI(Actor* a) : ap(a)
{
    this->ap = a;
}

AI::~AI()
{
}

RandomAI::RandomAI(Actor* e) : AI(e)
{
}

RandomAI::~RandomAI()
{
    
}

TurnAction RandomAI::make_turn(Map& m, Entity* player, std::mt19937& rd)
{
    /* AI steers entity directly */
    std::uniform_int_distribution<std::mt19937::result_type> randPosX(-1, 1);
    std::uniform_int_distribution<std::mt19937::result_type> randPosY(-1, 1);

    TurnAction turnResults = this->ap->ownEntity->moveOrPerformAction(randPosX(rd), randPosY(rd), m.blockingEntities2DVector, m.blockingEntities);
    
    return turnResults;
    
}
