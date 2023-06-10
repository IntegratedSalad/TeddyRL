//
//  utils.cpp
//  TeddyRL
//
//  Created by Dev on 20/03/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "utils.hpp"

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
