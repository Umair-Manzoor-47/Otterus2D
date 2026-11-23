#pragma once
#include "ComponentSerializer.h"

namespace otterus_core::ECS
{
    template <typename TComponent, typename TSerializer>
        void ComponentSerializer::Serialize(TSerializer& serializer, const TComponent& component)
    {
        SerializeComponent(serializer, component);
    }

    template <typename TComponent, typename TTable>
    inline void ComponentSerializer::Deserialize( const TTable& table, TComponent& component )
    {
        DeserializeComponent( table, component );
    }
}
