//
//  actor.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "actor.hpp"

Actor::Actor() : ai(nullptr)
{
    /* We have to set the ai pointer to nullptr, because player also has an actor component.
       Actor component has a pointer to an AI, that ISN'T AUTOMATICALLY SET TO NULLPTR!
     */
}

Actor::~Actor()
{
    if (this->ai != nullptr)
    {
        delete this->ai;
    }
    this->ai = nullptr;
    assert(this->ai == nullptr);
}
