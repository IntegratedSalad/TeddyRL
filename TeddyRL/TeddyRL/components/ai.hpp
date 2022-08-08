//
//  ai.hpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef ai_hpp
#define ai_hpp

#include "map.hpp"

class AI
{
private:
    
public:
    
    AI();
    ~AI();
    virtual void make_turn() = 0;
};

#endif /* ai_hpp */
