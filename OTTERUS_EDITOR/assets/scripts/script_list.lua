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

local function resolve_script(path)
    if script_exists(path) then
        return path
    end
    local alts = {
        "../" .. path,
        "OTTERUS_EDITOR/" .. path,
        path:gsub("^%.%./Games/", "assets/Games/"),
        path:gsub("^Games/", "assets/Games/"),
        path:gsub("^assets/scripts/Platformer/", "assets/Games/Platformer/"),
        path:gsub("^assets/scripts/CozyScene/", "assets/Games/CozyScene/"),
        path:gsub("^assets/scripts/asteroids/", "assets/Games/asteroids/"),
        "assets/Games/" .. path,
        "../assets/Games/" .. path
    }
    for _, alt in ipairs(alts) do
        if script_exists(alt) then
            return alt
        end
    end
    return path
end

local raw_script_list = {
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
    "assets/Games/Platformer/platformerMap.lua",
    "assets/Games/Platformer/platformer_data.lua",
    "assets/Games/Platformer/projectile.lua",
    "assets/Games/Platformer/player_factory.lua",
    "assets/Games/Platformer/platformer_systems.lua",
    "assets/Games/Platformer/player_states.lua",
    "assets/Games/Platformer/platformer_controller.lua",
    "assets/Games/Platformer/title_state.lua",
    "assets/Games/Platformer/game_state.lua",

    -- Asteroids Game Scripts
    "assets/Games/asteroids/entityDefs.lua",
    "assets/Games/asteroids/game_data.lua",
    "assets/Games/asteroids/utilities.lua",
    "assets/Games/asteroids/ship.lua",
    "assets/Games/asteroids/asteroid.lua",
    "assets/Games/asteroids/projectile.lua",
    "assets/Games/asteroids/collision_system.lua",
    "assets/Games/asteroids/title_state.lua",
    "assets/Games/asteroids/game_state.lua",
    "assets/Games/asteroids/game_over_state.lua",
    "assets/Games/asteroids/game.lua",
}

-- Optional Cozy Scene Scripts
local cozy_scripts = {
    "assets/Games/CozyScene/test_map.lua",
    "assets/Games/CozyScene/rain_generator.lua",
    "assets/Games/CozyScene/animal_wander.lua",
    "assets/Games/CozyScene/ambient_effects.lua",
    "assets/Games/CozyScene/day_night_cycle.lua",
    "assets/Games/CozyScene/cozy_scene.lua",
    "assets/Games/CozyScene/cozy_scene_state.lua",
}

script_list = {}

for _, script in ipairs(raw_script_list) do
    local resolved = resolve_script(script)
    if script_exists(resolved) then
        table.insert(script_list, resolved)
    end
end

for _, script in ipairs(cozy_scripts) do
    local resolved = resolve_script(script)
    if script_exists(resolved) then
        table.insert(script_list, resolved)
    end
end
