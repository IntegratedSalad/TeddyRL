//
//  drawing_utils.hpp
//  TeddyRL
//
//  Created by Dev on 22/09/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#ifndef drawing_utils_hpp
#define drawing_utils_hpp

#include <SFML/Graphics.hpp>

void DrawTextOnRectangle(sf::RenderWindow* window, sf::Color rectColor, sf::Color TextColor, const unsigned int textSize, const std::string& text, int x, int y, sf::Font& fontr);

void DrawText(sf::RenderWindow* window, sf::Color TextColor, const unsigned int textSize, const std::string& text, int x, int y, sf::Font& fontr);

#endif /* drawing_utils_hpp */
