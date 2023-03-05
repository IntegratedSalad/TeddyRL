//
//  handleKeys.cpp
//  TeddyRL
//
//  Created by Dev on 31/01/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "handleKeys.hpp"
#include <iostream>

/* KEY_PRESSED:ACTION_ENUM */

PlayerAction ReturnActionFromInput(const std::map<sf::Keyboard::Key, PlayerAction> bindingsMap, sf::Keyboard::Key key_code)
{
    PlayerAction action;
    try
    {
        action = bindingsMap.at(key_code);
    } catch (std::out_of_range)
    {
      action = PlayerAction::PLR_ACTION_IDLE;
    }
    return action;
}
