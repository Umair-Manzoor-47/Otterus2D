#pragma once
#include "Core/ECS/Components/AllComponents.h"
#include "physics/UserData.h"

namespace otterus_editor
{
    constexpr const char* DROP_TEXTURE_SRC = "DropTextureSource";
    constexpr const char* DROP_FONT_SRC = "DropFontSource";
    constexpr const char* DROP_SOUNDFX_SRC = "DropSoundFxSource";
    constexpr const char* DROP_MUSIC_SRC = "DropMusicSource";
    constexpr const char* DROP_SCENE_SRC = "DropSceneSource";


    struct Canvas
    {
        int width{ 640 }, height{ 480 }, tileWidth{ 16 }, tileHeight{ 16 };
    };

    struct Tile
    {
        uint32_t id{ entt::null };
        otterus_core::ECS::TransformComponent transform{};
        otterus_core::ECS::SpriteComponent sprite{};
        otterus_core::ECS::AnimationComponent animation{};
        otterus_core::ECS::BoxColliderComponent boxCollider{};
        otterus_core::ECS::CircleColliderComponent circleCollider{};
        otterus_core::ECS::PhysicsComponent physics{};
        otterus_physics::ObjectData objectData{};

        bool hasCollider{ false }, hasAnimation{ false }, enablePhysics{ false }, isCircle{ false };
    };

}
