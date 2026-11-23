#pragma once

#include "DrawComponentUtils.h"
#include "Core/ECS/Entity.h"

namespace otterus_editor
{

    template <typename TComponent>
    inline void DrawComponentUtils::DrawEntityComponentInfo( otterus_core::ECS::Entity& entity )
    {
        auto& component = entity.GetComponent<TComponent>();
        DrawImGuiComponent( component );
    }

    template <typename TComponent>
    inline void DrawComponentUtils::DrawComponentInfo( TComponent& component )
    {
        DrawImGuiComponent( component );
    }

    template <typename TComponent>
    inline void DrawComponentUtils::RegisterUIComponent()
    {
        using namespace entt::literals;

        entt::meta_factory<TComponent>()
            .type( entt::type_hash<TComponent>::value() )
            .template func<&DrawEntityComponentInfo<TComponent>>( "DrawEntityComponentInfo"_hs );
    }


}