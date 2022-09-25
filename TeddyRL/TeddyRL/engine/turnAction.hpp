//
//  turnAction.hpp
//  TeddyRL
//
//  Created by Dev on 25/09/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#ifndef turnAction_h
#define turnAction_h

// TODO: think about when there are multiple actions in turn, e.g. something opens and falls in the same turn etc.

/* Simple struct holding information about an action being performed. */
typedef struct TurnAction
{
    std::string name; // TODO: make an enum, not string.
    int entityPerformingActionVectorPos;
    int entityTargetOfActionVectorPos;
    
} TurnAction;


#endif /* turnAction_h */
