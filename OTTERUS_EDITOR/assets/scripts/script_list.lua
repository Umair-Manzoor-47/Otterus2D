-- Script Loading Manifest
-- Ordered list of all Lua scripts loaded into Otterus2D

local function script_exists(path)
    local f = io.open(path, "r")
    if f ~= nil then
        io.close(f)
        return true
    end
    return false
end

script_list = {
    -- Core Assets & Definitions
    "assets/scripts/asset_defs.lua",

    -- Shared Utilities API
    "assets/scripts/shared/utilities/math_utils.lua",
    "assets/scripts/shared/utilities/asset_loader.lua",
    "assets/scripts/shared/utilities/entity_loader.lua",
    "assets/scripts/shared/utilities/map_loader.lua",
    "assets/scripts/shared/utilities/follow_cam.lua",

    -- Shared Events & Trigger Systems
    "assets/scripts/shared/events/event_manager.lua",
    "assets/scripts/shared/events/collision_event.lua",
    "assets/scripts/shared/systems/trigger_system.lua",

    -- Platformer: Data, Factory, Systems, States & Controller
    "assets/scripts/Platformer/platformerMap.lua",
    "assets/scripts/Platformer/platformer_data.lua",
    "assets/scripts/Platformer/projectile.lua",
    "assets/scripts/Platformer/player_factory.lua",
    "assets/scripts/Platformer/platformer_systems.lua",
    "assets/scripts/Platformer/player_states.lua",
    "assets/scripts/Platformer/platformer_controller.lua",

    -- Scene States for StateStack
    "assets/scripts/Platformer/title_state.lua",
    "assets/scripts/Platformer/game_state.lua",
}

-- Optional Cozy Scene
local cozy_scripts = {
    "assets/scripts/CozyScene/test_map.lua",
    "assets/scripts/CozyScene/rain_generator.lua",
    "assets/scripts/CozyScene/animal_wander.lua",
    "assets/scripts/CozyScene/ambient_effects.lua",
    "assets/scripts/CozyScene/cozy_scene.lua",
    "assets/scripts/CozyScene/cozy_scene_state.lua",
}

for _, script in ipairs(cozy_scripts) do
    if script_exists(script) then
        table.insert(script_list, script)
    end
end
