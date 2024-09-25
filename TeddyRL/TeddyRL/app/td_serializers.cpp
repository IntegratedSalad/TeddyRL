//
//  td_serializers.cpp
//  TeddyRL
//
//  Created by Dev on 26/02/2023.
//  Copyright © 2023 Dev. All rights reserved.
//

#include "td_serializers.hpp"

Entity GetPlayerFromCollection(td_serialization_collection collection)
{
    Entity e;
    for (td_entity_serializer s : collection.entitySerializers)
    {
        if (s.entity.blockingEntitiesVectorPos == 0)
        {
            e = s.entity;
        }
    }
    return e;
}
