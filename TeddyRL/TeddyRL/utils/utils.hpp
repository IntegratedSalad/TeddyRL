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
#include "entity.hpp"
#include <SFML/Graphics.hpp>

typedef std::vector<std::vector<int>> Int2DVec;
void DebugPrintInt2dVector(const Int2DVec& vec, const std::string& name);
int DistanceBetweenTwoEntities(const Entity& e1, const Entity& e2);

#endif /* utils_hpp */
