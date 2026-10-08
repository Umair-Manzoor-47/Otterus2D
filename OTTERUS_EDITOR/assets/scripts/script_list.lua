-- Script Loading Manifest
-- Ordered list of all Lua scripts loaded into Otterus2D

script_list = {
    -- Core Assets & Definitions
    "assets/scripts/asset_defs.lua",

    -- Reusable Utilities API
    "assets/scripts/utilities/math_utils.lua",
    "assets/scripts/utilities/asset_loader.lua",
    "assets/scripts/utilities/entity_loader.lua",
    "assets/scripts/utilities/map_loader.lua",
    "assets/scripts/utilities/follow_cam.lua",

    -- Events & Trigger Systems
    "assets/scripts/events/event_manager.lua",
    "assets/scripts/events/collision_event.lua",
    "assets/scripts/systems/trigger_system.lua",

    -- Platformer: Data, Factory, Systems, States & Controller
    "assets/scripts/Platformer/platformerMap.lua",
    "assets/scripts/Platformer/platformer_data.lua",
    "assets/scripts/Platformer/player_factory.lua",
    "assets/scripts/Platformer/platformer_systems.lua",
    "assets/scripts/Platformer/player_states.lua",
    "assets/scripts/Platformer/platformer_controller.lua",

    -- Cozy Scene Showcase
    "assets/scripts/CozySceneShowcase/test_map.lua",
    "assets/scripts/CozySceneShowcase/rain_generator.lua",
    "assets/scripts/CozySceneShowcase/animal_wander.lua",
    "assets/scripts/CozySceneShowcase/ambient_effects.lua",
    "assets/scripts/CozySceneShowcase/cozy_scene.lua",
    "assets/scripts/CozySceneShowcase/cozy_scene_state.lua",

    -- Scene States for StateStack
    "assets/scripts/Platformer/title_state.lua",
    "assets/scripts/Platformer/game_state.lua",
}