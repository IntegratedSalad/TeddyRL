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

Action returnActionFromInput(const std::map<sf::Keyboard::Key, Action> bindingsMap, sf::Keyboard::Key key_code)
{
    Action action;
    try
    {
        action = bindingsMap.at(key_code);
    } catch (std::out_of_range)
    {
      action = Action::ACTION_IDLE;
    }
    
    return action;
}
