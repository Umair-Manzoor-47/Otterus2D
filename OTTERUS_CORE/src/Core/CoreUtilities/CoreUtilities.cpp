#include "CoreUtilities.h"
#include "Core/ECS/Components/AllComponents.h"


static const std::unordered_map<std::string, otterus_core::ECS::RigidBodyType> StringToRigidBodyType{
	    { "static", otterus_core::ECS::RigidBodyType::STATIC },
        { "kinematic", otterus_core::ECS::RigidBodyType::KINEMATIC },
        { "dynamic", otterus_core::ECS::RigidBodyType::DYNAMIC } };

static const std::unordered_map<otterus_core::ECS::RigidBodyType, std::string> RigidBodyTypeToString{
	    { otterus_core::ECS::RigidBodyType::STATIC, "static" },
        { otterus_core::ECS::RigidBodyType::KINEMATIC, "kinematic" },
        { otterus_core::ECS::RigidBodyType::DYNAMIC, "dynamic" } };


namespace otterus_core
{
    bool EntityInView( const ECS::TransformComponent& transform, float width, float height,
                                     const otterus_rendering::Camera2D& camera )
    {
        if ( camera.GetWidth() <= 0 || camera.GetHeight() <= 0 )
            return true;
        // Convert screen corners (0,0) and (width, height) to world coordinates
        const glm::vec2 camTopLeft = camera.ScreenCoordToWorld( glm::vec2{ 0.f, 0.f } );
        const glm::vec2 camBottomRight = camera.ScreenCoordToWorld( glm::vec2{
            static_cast<float>( camera.GetWidth() ),
            static_cast<float>( camera.GetHeight() )
        } );
        const float minX = std::min( camTopLeft.x, camBottomRight.x );
        const float maxX = std::max( camTopLeft.x, camBottomRight.x );
        const float minY = std::min( camTopLeft.y, camBottomRight.y );
        const float maxY = std::max( camTopLeft.y, camBottomRight.y );
        const float spriteWidth  = std::abs( width  * transform.scale.x );
        const float spriteHeight = std::abs( height * transform.scale.y );
        // Standard AABB overlap check
        if ( ( transform.position.x + spriteWidth <= minX ) ||
             ( transform.position.x >= maxX ) ||
             ( transform.position.y + spriteHeight <= minY ) ||
             ( transform.position.y >= maxY ) )
        {
            return false; // Outside camera view
        }
        return true; // Inside camera view
    }

    glm::mat4 RSTModel( const ECS::TransformComponent& transform, float width, float height )
    {
        glm::mat4 model{ 1.f };
        if ( transform.rotation > 0.f || transform.rotation < 0.f || transform.scale.x > 1.f || transform.scale.x < 1.f ||
             transform.scale.y > 1.f || transform.scale.y < 1.f )
        {
            model = glm::translate( model, glm::vec3{ transform.position, 0.f } );
            model = glm::translate(
                model, glm::vec3{ ( width * transform.scale.x ) * 0.5f, ( height * transform.scale.y ) * 0.5f, 0.f } );
            model = glm::rotate( model, glm::radians( transform.rotation ), glm::vec3{ 0.f, 0.f, 1.f } );
            model = glm::translate(
                model, glm::vec3{ ( width * transform.scale.x ) * -0.5f, ( height * transform.scale.y ) * -0.5f, 0.f } );
            model = glm::scale( model, glm::vec3{ transform.scale, 1.0f } );
            model = glm::translate( model, glm::vec3{ -transform.position, 0.f } );
        }

        return model;
    }

    std::string GetRigidBodyTypeString( ECS::RigidBodyType eRigidType )
    {
        auto rigidItr = RigidBodyTypeToString.find( eRigidType );
        if ( rigidItr == RigidBodyTypeToString.end() )
            return {};

        return rigidItr->second;
    }

    ECS::RigidBodyType GetRigidBodyTypeByString( const std::string sRigidType )
    {
        auto rigidItr = StringToRigidBodyType.find( sRigidType );
        if ( rigidItr == StringToRigidBodyType.end() )
            return ECS::RigidBodyType::STATIC;

        return rigidItr->second;
    }

    void GenerateUVs( ECS::SpriteComponent& sprite, int textureWidth, int textureHeight )
    {
        sprite.uvs.uv_width = sprite.width / textureWidth;
        sprite.uvs.uv_height = sprite.height / textureHeight;

        sprite.uvs.u = sprite.start_x * sprite.uvs.uv_width;
        sprite.uvs.v = sprite.start_y * sprite.uvs.uv_height;
    }
}
