//
//  actor.cpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#include "actor.hpp"

Actor::Actor()
{
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
