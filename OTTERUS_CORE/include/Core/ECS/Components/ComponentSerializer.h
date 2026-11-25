#pragma once
#include "AllComponents.h"
#include <rapidjson/document.h>

namespace otterus_filesystem
{
    class JSONSerializer;
}

#define SERIALIZE_COMPONENT( serializer, component )                           \
otterus_core::ECS::ComponentSerializer::Serialize( serializer, component )

#define DESERIALIZE_COMPONENT( table, compref )                                \
otterus_core::ECS::ComponentSerializer::Deserialize( table, compref )

namespace otterus_core::ECS
{
    class ComponentSerializer
    {
    public:
        ComponentSerializer() = delete;

        template <typename TComponent, typename TSerializer>
        static void Serialize( TSerializer& serializer, const TComponent& component );

        template <typename TComponent, typename TTable>
        static void Deserialize( const TTable& table, TComponent& component );;
    private:
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const TransformComponent& transform );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const SpriteComponent& sprite );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const AnimationComponent& animation );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
                                        const BoxColliderComponent& boxCollider );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
                                        const CircleColliderComponent& circleCollider );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const TextComponent& text );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const PhysicsComponent& physics );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const RigidBodyComponent& rigidBody );
        static void SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const LightComponent& light );

        static void DeserializeComponent( const rapidjson::Value& jsonValue, TransformComponent& transform );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, SpriteComponent& sprite );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, AnimationComponent& animation );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, BoxColliderComponent& boxCollider );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, CircleColliderComponent& circleCollider );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, TextComponent& text );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, PhysicsComponent& physics );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, RigidBodyComponent& rigidBody );
        static void DeserializeComponent( const rapidjson::Value& jsonValue, LightComponent& light );

    };

}

#include "ComponentSerializer.inl"