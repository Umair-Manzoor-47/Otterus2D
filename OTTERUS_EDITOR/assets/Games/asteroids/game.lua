math.randomseed(os.time())

local function resolve_dir()
    local candidates = {
        "assets/Games/asteroids/",
        "../assets/Games/asteroids/",
        "OTTERUS_EDITOR/assets/Games/asteroids/",
        "Games/asteroids/",
        "../Games/asteroids/"
    }
    for _, dir in ipairs(candidates) do
        local f = io.open(dir .. "ship.lua", "r")
        if f then
            f:close()
            return dir
        end
    end
    return "assets/Games/asteroids/"
end

local base = resolve_dir()

run_script("assets/scripts/shared/utilities/math_utils.lua")
run_script("assets/scripts/shared/events/event_manager.lua")
run_script("assets/scripts/shared/events/collision_event.lua")

run_script(base .. "entityDefs.lua")
run_script(base .. "assetDef.lua")
run_script(base .. "game_data.lua")
run_script(base .. "utilities.lua")
run_script(base .. "projectile.lua")
run_script(base .. "asteroid.lua")
run_script(base .. "ship.lua")
run_script(base .. "collision_system.lua")
run_script(base .. "title_state.lua")
run_script(base .. "game_state.lua")
run_script(base .. "game_over_state.lua")

LoadAsteroidsAssets()

local stack = StateStack()
gStateStack = stack
stack:change_state(AsteroidsTitleState:Create(stack))

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
