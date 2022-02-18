//
//  handleKeys.cpp
//  TeddyRL
//
//  Created by Dev on 31/01/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "handleKeys.hpp"

/* KEY_PRESSED:ACTION_ENUM */


/* Always return an action accessed by a key being a lowercase string. */
Action returnActionFromInput(const std::map<std::string, Action> bindingsMap)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        return bindingsMap.at("h");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        return bindingsMap.at("l");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::K))
    {
        return bindingsMap.at("k");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::J))
    {
        return bindingsMap.at("j");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
    {
        return bindingsMap.at("u");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
    {
        return bindingsMap.at("y");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
    {
        return bindingsMap.at("n");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
    {
        return bindingsMap.at("b");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        return bindingsMap.at("esc");
    }

}

std::string getPlayerAction()
{
    
}
