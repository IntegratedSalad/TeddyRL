//
//  actor.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "actor.hpp"

Actor::Actor() : ai(nullptr), hp(0), mana(0)
{
    /* We have to set the ai pointer to nullptr, because player also has an actor component.
       Actor component has a pointer to an AI, that ISN'T AUTOMATICALLY SET TO NULLPTR!
     */
    // TODO: Setup of the ai here
}

Actor::Actor(const Actor& ac) : hp(ac.hp), mana(ac.mana), typeOfAI(ac.typeOfAI)
{
    this->SetupAI(this->typeOfAI);
}

Actor::~Actor()
{
//    if (this->ai != nullptr)
//    {
//        delete this->ai;
//    }
    
    // Maybe AI has been destroyed previously?
    this->ai = nullptr;
    assert(this->ai == nullptr);
}

void Actor::SetupAI(AIType t)
{
    AI* newAIp = nullptr;
    switch (t)
    {
        case AIType::RANDOM:
        {
            RandomAI* raip = new RandomAI();
            newAIp = raip;
            setAIType(AIType::RANDOM);
            break;
        }
        case AIType::NONE:
        {
            setAIType(AIType::NONE);
            break;
        }
        default:
        {
            setAIType(AIType::NONE);
            break;
        }
    }
    this->setAI(newAIp);
}
