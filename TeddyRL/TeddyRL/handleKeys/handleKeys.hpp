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
#include "PlayerActions.h"
#define DEBUG_ACTION 404

using namespace sf;
typedef std::map<Keyboard::Key, PlayerAction> KeyActionMap;
const KeyActionMap inGameBindings =
{

    {Keyboard::Key::K, PlayerAction::PLR_ACTION_MOVE_N},
    {Keyboard::Key::Up, PlayerAction::PLR_ACTION_MOVE_N},
    {Keyboard::Key::J, PlayerAction::PLR_ACTION_MOVE_S},
    {Keyboard::Key::Down, PlayerAction::PLR_ACTION_MOVE_S},
    {Keyboard::Key::Y, PlayerAction::PLR_ACTION_MOVE_NW},
    {Keyboard::Key::U, PlayerAction::PLR_ACTION_MOVE_NE},
    {Keyboard::Key::L, PlayerAction::PLR_ACTION_MOVE_E},
    {Keyboard::Key::Right, PlayerAction::PLR_ACTION_MOVE_E},
    {Keyboard::Key::H, PlayerAction::PLR_ACTION_MOVE_W},
    {Keyboard::Key::Left, PlayerAction::PLR_ACTION_MOVE_W},
    {Keyboard::Key::N, PlayerAction::PLR_ACTION_MOVE_SE},
    {Keyboard::Key::B, PlayerAction::PLR_ACTION_MOVE_SW},
    {Keyboard::Key::Period, PlayerAction::PLR_ACTION_PASS_TURN},
#if 1
    {Keyboard::Key::F1, static_cast<PlayerAction>(DEBUG_ACTION)}
#endif
};

PlayerAction returnActionFromInput(const KeyActionMap bindingsMap, Keyboard::Key key_code);

#endif /* handleKeys_hpp */
