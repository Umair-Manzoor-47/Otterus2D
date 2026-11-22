math.randomseed(os.time())

local function resolve_dir()
    local candidates = {
        "assets/Games/Platformer/",
        "../assets/Games/Platformer/",
        "OTTERUS_EDITOR/assets/Games/Platformer/",
        "Games/Platformer/",
        "../Games/Platformer/"
    }
    for _, dir in ipairs(candidates) do
        local f = io.open(dir .. "platformerMap.lua", "r")
        if f then
            f:close()
            return dir
        end
    end
    return "assets/Games/Platformer/"
end

local base = resolve_dir()

run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/shared/utilities/math_utils.lua")
run_script("assets/scripts/shared/utilities/asset_loader.lua")
run_script("assets/scripts/shared/utilities/entity_loader.lua")
run_script("assets/scripts/shared/utilities/map_loader.lua")
run_script("assets/scripts/shared/utilities/follow_cam.lua")
run_script("assets/scripts/shared/events/event_manager.lua")
run_script("assets/scripts/shared/events/collision_event.lua")
run_script("assets/scripts/shared/systems/trigger_system.lua")

run_script(base .. "platformerMap.lua")
run_script(base .. "platformer_data.lua")
run_script(base .. "projectile.lua")
run_script(base .. "player_factory.lua")
run_script(base .. "platformer_systems.lua")
run_script(base .. "player_states.lua")
run_script(base .. "platformer_controller.lua")
run_script(base .. "title_state.lua")
run_script(base .. "game_state.lua")

LoadAssets(AssetDefs)

local stack = StateStack()
gStateStack = stack
stack:change_state(PlatformerTitleState:Create(stack, nil))

main = {
    [1] = {
        update = function()
            stack:update(0.016)
        end
    },
    [2] = {
        render = function()
        end
    }
}
