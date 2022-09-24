//
//  drawing_utils.cpp
//  TeddyRL
//
//  Created by Dev on 22/09/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "drawing_utils.hpp"


void drawTextOnRectangle(sf::RenderWindow* window, sf::Color rectColor, sf::Color TextColor, const unsigned int textSize, const std::string& text, int x, int y, sf::Font& fontr)
{
    sf::Text textToDraw;
    textToDraw.setFont(fontr);
    textToDraw.setString(text);
    textToDraw.setCharacterSize(textSize);
    textToDraw.setFillColor(TextColor);
    textToDraw.setPosition(x, y);
    
    sf::FloatRect textRect;
    textRect = textToDraw.getGlobalBounds();
    
    sf::RectangleShape rectToDraw;
    sf::Vector2f vSize{textRect.width, textRect.height + 10};
    rectToDraw.setSize(vSize);
    rectToDraw.setFillColor(sf::Color::Black);
    rectToDraw.setPosition(x, y + 8);
    
    window->draw(rectToDraw);
    window->draw(textToDraw);
}
