//
//  ai.cpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "ai.hpp"

AI::AI()
{
}

AI::~AI()
{
}

RandomAI::RandomAI()
{
}

RandomAI::~RandomAI()
{
}

ActionResult RandomAI::make_turn(Map& m, Entity* player, std::mt19937& rd)
{
#warning Using Marsenne Twister for random movement is very inefficient!
    std::uniform_int_distribution<std::mt19937::result_type> randDir(0, 8);
    ActionResult aiActionResult;
    aiActionResult.type = ActionType::ACTIONTYPE_MOVE;
    aiActionResult.m_ActionData["direction"] = randDir(rd);
    return aiActionResult;
}
