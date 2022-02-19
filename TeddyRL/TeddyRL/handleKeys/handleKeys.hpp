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

static const std::map<sf::Keyboard::Key, Action> in_game_bindings =
{
    {sf::Keyboard::Key::K, Action::ACTION_MOVE_N},
    {sf::Keyboard::Key::Up, Action::ACTION_MOVE_N},
    {sf::Keyboard::Key::J, Action::ACTION_MOVE_S},
    {sf::Keyboard::Key::Down, Action::ACTION_MOVE_S},
    {sf::Keyboard::Key::Y, Action::ACTION_MOVE_NW},
    {sf::Keyboard::Key::U, Action::ACTION_MOVE_NE},
    {sf::Keyboard::Key::L, Action::ACTION_MOVE_E},
    {sf::Keyboard::Key::Right, Action::ACTION_MOVE_E},
    {sf::Keyboard::Key::H, Action::ACTION_MOVE_W},
    {sf::Keyboard::Key::Left, Action::ACTION_MOVE_W},
    {sf::Keyboard::Key::N, Action::ACTION_MOVE_SE},
    {sf::Keyboard::Key::B, Action::ACTION_MOVE_SW}
};

Action returnActionFromInput(const std::map<sf::Keyboard::Key, Action> bindingsMap, sf::Keyboard::Key key_code);

#endif /* handleKeys_hpp */
