add_library(OTTERUS_CORE STATIC
    # Utils
    src/Core/CoreUtilities/CoreEngineData.cpp
    src/Core/CoreUtilities/CoreUtilities.cpp
    src/Core/CoreUtilities/FollowCamera.cpp
    include/Core/CoreUtilities/CoreEngineData.h
    include/Core/CoreUtilities/CoreUtilities.h
    include/Core/CoreUtilities/FollowCamera.h

    # Components
    src/Core/ECS/Components/AnimationComponent.cpp
    src/Core/ECS/Components/BoxColliderComponent.cpp
    src/Core/ECS/Components/CircleColliderComponent.cpp
    src/Core/ECS/Components/PhysicsComponent.cpp
    src/Core/ECS/Components/RigidBodyComponent.cpp
    src/Core/ECS/Components/SpriteComponent.cpp
    src/Core/ECS/Components/TextComponent.cpp
    src/Core/ECS/Components/TransformComponent.cpp
    include/Core/ECS/Components/AnimationComponent.h
    include/Core/ECS/Components/BoxColliderComponent.h
    include/Core/ECS/Components/CircleColliderComponent.h
    include/Core/ECS/Components/Identification.h
    include/Core/ECS/Components/PhysicsComponent.h
    include/Core/ECS/Components/RigidBodyComponent.h
    include/Core/ECS/Components/ScriptComponent.h
    include/Core/ECS/Components/SpriteComponent.h
    include/Core/ECS/Components/TextComponent.h
    include/Core/ECS/Components/TileComponent.h
    include/Core/ECS/Components/TransformComponent.h
    include/Core/ECS/Components/AllComponents.h

    src/Core/ECS/Entity.cpp
    src/Core/ECS/MainRegistry.cpp
    src/Core/ECS/MetaUtilities.cpp
    src/Core/ECS/Registry.cpp
    include/Core/ECS/Entity.h
    include/Core/ECS/Entity.inl
    include/Core/ECS/MainRegistry.h
    include/Core/ECS/MetaUtilities.h
    include/Core/ECS/Registry.h
    include/Core/ECS/Registry.inl

    # Scripting
    src/Core/Scripting/ContactListenerBind.cpp
    src/Core/Scripting/GlmLuaBindings.cpp
    src/Core/Scripting/InputManager.cpp
    src/Core/Scripting/RendererBindings.cpp
    src/Core/Scripting/SoundBindings.cpp
    src/Core/Scripting/UserDataBindings.cpp
    include/Core/Scripting/ContactListenerBind.h
    include/Core/Scripting/GlmLuaBindings.h
    include/Core/Scripting/InputManager.h
    include/Core/Scripting/RendererBindings.h
    include/Core/Scripting/SoundBindings.h
    include/Core/Scripting/UserDataBindings.h
    include/Core/Scripting/UserDataBindings.inl

    # State Machine
    src/Core/States/State.cpp
    src/Core/States/StateMachine.cpp
    src/Core/States/StateStack.cpp
    include/Core/States/State.h
    include/Core/States/StateStack.h
    include/Core/States/StateMachine.h

    # Systems
    src/Core/Systems/AnimationSystem.cpp
    src/Core/Systems/PhysicsSystem.cpp
    src/Core/Systems/RenderShapeSystem.cpp
    src/Core/Systems/RenderSystem.cpp
    src/Core/Systems/RenderUISystem.cpp
    src/Core/Systems/ScriptingSystem.cpp
    include/Core/Systems/AnimationSystem.h
    include/Core/Systems/PhysicsSystem.h
    include/Core/Systems/RenderShapeSystem.h
    include/Core/Systems/RenderSystem.h
    include/Core/Systems/RenderUISystem.h
    include/Core/Systems/ScriptingSystem.h

    # Resources
    src/Core/Resources/AssetManager.cpp
    include/Core/Resources/AssetManager.h
    include/Core/Resources/fonts/default_font.h
)

target_include_directories(OTTERUS_CORE PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(OTTERUS_CORE PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/CoreUtilities"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/ECS"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/ECS/Components"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/Resources"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/Scripting"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/States"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/Systems"
)

target_link_libraries(OTTERUS_CORE PUBLIC
    OTTERUS_LOGGER
    OTTERUS_UTILITIES
    OTTERUS_WINDOW
    OTTERUS_SOUND
    OTTERUS_RENDERING
    OTTERUS_PHYSICS
    EnTT::EnTT
    GLM::GLM
    Sol3::Sol3
    Lua53::Lua53
    SDL2::SDL2
    SDL2::SDL2_mixer
    Box2D::Box2D
)
