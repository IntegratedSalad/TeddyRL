//
//  handleKeys.hpp
//  TeddyRL
//
//  Created by Dev on 31/01/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#ifndef handleKeys_hpp
#define handleKeys_hpp

#include <SFML/Window.hpp>
#include <map>

#include "Actions.h"

static const std::map<std::string, Action> in_game_bindings =
{
    {"k", Action::ACTION_MOVE_N},
    {"j", Action::ACTION_MOVE_S},
    {"y", Action::ACTION_MOVE_NW},
    {"u", Action::ACTION_MOVE_NE},
    {"l", Action::ACTION_MOVE_E},
    {"h", Action::ACTION_MOVE_W},
    {"n", Action::ACTION_MOVE_SE},
    {"b", Action::ACTION_MOVE_SW}
};

Action returnActionFromInput(const std::map<std::string, Action> bindingsMap);

#endif /* handleKeys_hpp */
