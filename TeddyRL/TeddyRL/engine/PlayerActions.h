//
//  PlayerActions.h
//  TeddyRL
//
//  Created by Dev on 18/02/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#ifndef PlayerActions_h
#define PlayerActions_h

/* All possible player actions across the entire game */
enum class PlayerAction
{
    /*  Moving  */
    PLR_ACTION_MOVE_N,
    PLR_ACTION_MOVE_NE,
    PLR_ACTION_MOVE_E,
    PLR_ACTION_MOVE_SE,
    PLR_ACTION_MOVE_S,
    PLR_ACTION_MOVE_SW,
    PLR_ACTION_MOVE_W,
    PLR_ACTION_MOVE_NW,
    PLR_ACTION_IDLE,
    PLR_ACTION_PASS_TURN
    /*          */
};

#endif /* PlayerActions_h */
