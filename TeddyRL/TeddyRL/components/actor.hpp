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
#include "ai.hpp"
#include "entity.hpp"
#include "TurnActions.hpp"
#include <random>
#include <boost/archive/binary_oarchive.hpp>

/* Actor is a component that relies on AI that it's given */
typedef std::vector<std::vector<int>> Int2DVec;
class Entity;
class Map;
class AI;
enum class AIType;

class Actor
{
private:
    int hp;
    int mana;
    AI* ai;
    AIType typeOfAI;
    
    /*
     Serialization block.
     AI pointer is NOT serialized (its value).
     */
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & hp;
        ar & mana;
        ar & typeOfAI;
    }
    
public:
    Actor();
    Actor(const Actor&);
    ~Actor();
    
    void setAI(AI* aip) {this->ai = aip;}
    void setAIType(AIType t) {this->typeOfAI = t;}
    void SetupAI(AIType t);
    
    AIType GetType(void) {return this->typeOfAI;}
    AI* getAI(void) {return this->ai;}
    
    ActionResult attack(const Actor&) const; // attack method doesn't affect Actor directly.

#warning Remember this, when designing entity that has FSM.
    /*
      Serializing Actor's AI has to serialize its next move if he has any strategy!
     */
};

#endif /* actor_hpp */
