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

using namespace sf;
typedef std::map<Keyboard::Key, Action> KeyActionMap;
static const KeyActionMap in_game_bindings =
{

    {Keyboard::Key::K, Action::ACTION_MOVE_N},
    {Keyboard::Key::Up, Action::ACTION_MOVE_N},
    {Keyboard::Key::J, Action::ACTION_MOVE_S},
    {Keyboard::Key::Down, Action::ACTION_MOVE_S},
    {Keyboard::Key::Y, Action::ACTION_MOVE_NW},
    {Keyboard::Key::U, Action::ACTION_MOVE_NE},
    {Keyboard::Key::L, Action::ACTION_MOVE_E},
    {Keyboard::Key::Right, Action::ACTION_MOVE_E},
    {Keyboard::Key::H, Action::ACTION_MOVE_W},
    {Keyboard::Key::Left, Action::ACTION_MOVE_W},
    {Keyboard::Key::N, Action::ACTION_MOVE_SE},
    {Keyboard::Key::B, Action::ACTION_MOVE_SW}
};

Action returnActionFromInput(const KeyActionMap bindingsMap, Keyboard::Key key_code);

#endif /* handleKeys_hpp */
