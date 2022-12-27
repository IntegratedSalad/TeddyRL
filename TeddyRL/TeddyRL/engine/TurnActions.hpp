//
//  TurnActions.hpp
//  TeddyRL
//
//  Created by Dev on 25/09/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

/* Entity actions data structures */

#ifndef TurnActions_h
#define TurnActions_h
#include <tuple>
#include <map>

// TODO: think about when there are multiple actions in turn, e.g. something opens and falls in the same turn etc.

enum class ActionType
{
    ACTIONTYPE_MOVE = 0,
    ACTIONTYPE_ATTACK,
    ACTIONTYPE_IDLE
};

/* Simple struct holding information about an action being performed. */
typedef struct ActionResult
{
    ActionType type;
    int entityPerformingActionVectorPos; // information of where to find this entity in blockingEntities vector
    int entityTargetOfActionVectorPos;
    std::map<std::string, unsigned int> m_ActionData;
} ActionResult;

enum class Direction
{
    DIR_N = 0,
    DIR_NE,
    DIR_E,
    DIR_SE,
    DIR_S,
    DIR_WS,
    DIR_W,
    DIR_NW,
    DIR_IDLE
};

std::tuple<int, int> mapDirectionToCoordinates(Direction dir);

#endif /* TurnActions_h */
