//
//  utils.hpp
//  TeddyRL
//
//  Created by Dev on 20/03/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <SFML/Graphics.hpp>

typedef enum class DieToThrow
{
    D4,
    D6,
    D8,
    D10,
    D00,
    D12,
    D20
} DieToThrow;

class Entity;
typedef std::vector<std::vector<int>> Int2DVec;
void DebugPrintInt2dVector(const Int2DVec& vec, const std::string& name);

// TODO: Create vector and use sf::Vector2 to calculate distance
int DistanceBetweenTwoEntities(const Entity& e1, const Entity& e2);

// TODO: BresenhamLine

unsigned int rollDie(DieToThrow, std::mt19937&);
unsigned int randomNumInRange(unsigned int low, unsigned int high, std::mt19937& mtRef);

// TODO:  Make general assertion function

#endif /* utils_hpp */
