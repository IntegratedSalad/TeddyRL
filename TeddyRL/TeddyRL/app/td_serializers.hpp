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
//#include <boost/archive/binary_object.hpp>

// Make template if more enums will be needed to be serialized
static inline unsigned int TileSpriteToInt(TileSprite enumVal)
{
    return static_cast<int>(enumVal);
}

struct td_actor_serializer
{
    
};

struct td_entity_serializer
{
    
    friend class boost::serialization::access;
    Entity entity;
    unsigned int spriteIntEnumVal;
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & entity;
        ar & spriteIntEnumVal;
    }
    
    td_entity_serializer(Entity e, TileSprite ts) : entity(e)
    {
        this->spriteIntEnumVal = TileSpriteToInt(ts);
    };
    
    // TODO: Make custom deserialization if needed
    
    td_entity_serializer() { };
    
};

namespace boost {
namespace serialization {


} // namespace serialization
} // namespace boost


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
    void save(Archive& ar, const unsigned int version) const
    {
        ar & entitySerializers;
        ar & serializedMap; // calls constructor with spritesVector
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
        ar & entitySerializers;
        ar & serializedMap;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER(); // in case there's future need for differentiating between saving and loading collection
    
};

// SPLITTING SAVING AND LOADING:


#endif /* td_serializers_hpp */
