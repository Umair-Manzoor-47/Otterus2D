#pragma once
#include "Core/ECS/Components/AllComponents.h"
#include <Physics/PhysicsUtilities.h>
#include <Rendering/Core/Camera2D.h>
namespace otterus_core
{
    bool EntityInView( const ECS::TransformComponent& transform, float width, float height,
                       const otterus_rendering::Camera2D& camera );
    glm::mat4 RSTModel( const ECS::TransformComponent& transform, float width, float height );

    std::string GetRigidBodyTypeString( otterus_physics::RigidBodyType eRigidType );
    otterus_physics::RigidBodyType GetRigidBodyTypeByString( const std::string sRigidType );
    void GenerateUVs( ECS::SpriteComponent& sprite, int textureWidth, int textureHeight );
}