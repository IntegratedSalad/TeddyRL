//
//  utils.cpp
//  TeddyRL
//
//  Created by Dev on 20/03/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "utils.hpp"
#include "entity.hpp"

void DebugPrintInt2dVector(const Int2DVec& vec, const std::string& name)
{
    std::cout << ">>>> " << name << " vec contents: <<<<" << std::endl;
    for (Int2DVec::const_iterator it = vec.begin(); it != vec.end(); it++)
    {
        for (std::vector<int>::const_iterator jt = it->begin(); jt != it->end(); jt++)
        {
            std::cout << *jt << " ";
        }
        std::cout << std::endl;
    }
}

/* TODO: Pass a vector or two points, not whole entities */
int DistanceBetweenTwoEntities(const Entity& e1, const Entity& e2)
{
    return sqrt( pow(abs(e1.GetY() - e2.GetY()), 2.0) + pow(abs(e1.GetX() - e2.GetX()), 2.0) );
}

// Maybe write function returnRNFromRange or something.

typedef std::uniform_int_distribution<std::mt19937::result_type> RNG;
unsigned int rollDie(DieToThrow dType, std::mt19937& mtRef)
{
    unsigned int result = 0;
    switch (dType)
    {
        case DieToThrow::D4:
        {
            RNG d4(1, 4);
            result = d4(mtRef);
            break;
        }
        case DieToThrow::D6:
        {
            RNG d6(1, 6);
            result = d6(mtRef);
            break;
        }
        case DieToThrow::D8:
        {
            RNG d8(1, 8);
            result = d8(mtRef);
            break;
        }
        case DieToThrow::D10:
        {
            RNG d10(1, 10);
            result = d10(mtRef);
            break;
        }
        case DieToThrow::D00:
        {
            // TODO: Implement D00
            std::cout << "D00 Not implemented." << std::endl;
            break;
        }
        case DieToThrow::D12:
        {
            RNG d12(1, 12);
            result = d12(mtRef);
            break;
        }
        case DieToThrow::D20:
        {
            RNG d20(1, 20);
            result = d20(mtRef);
            break;
        }
        default:
            break;
    }
    assert(result >= 1);
    return result;
}

unsigned int randomNumInRange(unsigned int low, unsigned int high, std::mt19937& mtRef)
{
    unsigned int result = 0;
    RNG randInt(low, high);
    result = randInt(mtRef);
    return result;
}

