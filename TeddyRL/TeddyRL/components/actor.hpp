//
//  actor.hpp
//  TeddyRL
//
//  Created by Dev on 13/11/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef actor_hpp
#define actor_hpp

#include <memory>
#include <SFML/Graphics.hpp>

/* Actor is a component that relies on AI that it's given */

class Actor
{
private:
    
    int hp;
    int mana;
    
    
    
public:
    
    Actor();
    ~Actor();
    
    void make_turn();
    
};

#endif /* actor_hpp */
