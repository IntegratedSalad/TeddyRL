//
//  TurnActions.cpp
//  TeddyRL
//
//  Created by Dev on 26/12/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "TurnActions.hpp"

std::tuple<int, int> MapDirectionToCoordinates(Direction dir)
{
    int x = 0;
    int y = 0;
    
    switch (dir)
    {
        case Direction::DIR_N:
        {
            y = 1;
            break;
        }
        case Direction::DIR_NE:
        {
            x = 1;
            y = 1;
            break;
        }
        case Direction::DIR_E:
        {
            x = 1;
            break;
        }
        case Direction::DIR_SE:
        {
            y = -1;
            x = 1;
            break;
        }
        case Direction::DIR_S:
        {
            y = -1;
            break;
        }
        case Direction::DIR_WS:
        {
            y = -1;
            x = -1;
            break;
        }
        case Direction::DIR_W:
        {
            x = -1;
            break;
        }
        case Direction::DIR_NW:
        {
            y = 1;
            x = -1;
            break;
        }
        case Direction::DIR_IDLE:
        {break;}
    }
    return std::make_tuple(x, y);
}
