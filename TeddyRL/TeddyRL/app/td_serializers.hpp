//
//  td_serializers.hpp
//  TeddyRL
//
//  Created by Dev on 26/02/2023.
//  Copyright © 2023 Dev. All rights reserved.
//

#ifndef td_serializers_hpp
#define td_serializers_hpp

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "entity.hpp"
#include "map.hpp"

#define TD_SER_NEEDS_ACTOR_COMP 0x1

enum class AIType;

inline unsigned int AITypeToUInt(AIType ait)
{
    return static_cast<unsigned int>(ait);
}

// Make template if more enums will be needed to be serialized
static inline unsigned int TileSpriteToUInt(TileSprite enumVal)
{
    return static_cast<unsigned int>(enumVal);
}

inline TileSprite UIntToTileSprite(unsigned int id)
{
    return static_cast<TileSprite>(id);
}

// TODO: think of a function that restores entity.

struct td_actor_serializer
{
    
};

struct td_entity_serializer
{
    
    friend class boost::serialization::access;
    Entity entity;
    unsigned int spriteIntEnumVal;
    Actor actor;
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & entity;
        ar & spriteIntEnumVal;
        ar & actor;
        ar & FLAGS;
    }
    
    td_entity_serializer(Entity e, TileSprite ts, Actor a) : entity(e), actor(a)
    {
        this->spriteIntEnumVal = TileSpriteToUInt(ts);
    };
    
    // TODO: Make custom deserialization if needed
    
    td_entity_serializer() { };
    
    void SetEntityToSerialize(Entity e) { this->entity = e;}
    void SetTileSpriteToSerialize(TileSprite ts) { this->spriteIntEnumVal = TileSpriteToUInt(ts);}
    void SetActorToSerialize(Actor a) { this->actor = a;}
    
    unsigned char FLAGS = 0;
};

struct td_serialization_collection
{
    friend class boost::serialization::access;
    /*
     Vector of entity serializers. They have a copy of the entity serialized and its identifier in the spriteVector.
     */
    std::vector<td_entity_serializer> entitySerializers;
    
    /*
     Map object to be serialized.
     Map doesn't serialize its entityVector (blockingEntities)
     */
    Map serializedMap;
    
    td_serialization_collection(Map m, std::vector<td_entity_serializer> evs) : serializedMap(m), entitySerializers(evs) { };
    td_serialization_collection() { };
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & entitySerializers;
        ar & serializedMap;
    }
};

#endif /* td_serializers_hpp */
