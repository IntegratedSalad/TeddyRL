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
#include <fstream>
#include <math.h>
#include <random>
#include "entity.hpp"
#include <SFML/Graphics.hpp>

typedef enum class Die
{
    D4,
    D6,
    D8,
    D10,
    D00,
    D12,
    D20
} Die;

typedef std::vector<std::vector<int>> Int2DVec;
void DebugPrintInt2dVector(const Int2DVec& vec, const std::string& name);
int DistanceBetweenTwoEntities(const Entity& e1, const Entity& e2);

// TODO: BresenhamLine

unsigned int rollDie(Die, std::mt19937&);
unsigned int randomNumInRange(unsigned int low, unsigned int up, std::mt19937& mtRef);

#endif /* utils_hpp */
