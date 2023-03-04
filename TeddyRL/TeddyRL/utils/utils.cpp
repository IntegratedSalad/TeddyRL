//
//  utils.cpp
//  TeddyRL
//
//  Created by Dev on 20/03/2022.
//  Copyright © 2022 Dev. All rights reserved.
//

#include "utils.hpp"

void debugPrintInt2dVector(const Int2DVec& vec, const std::string& name)
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
