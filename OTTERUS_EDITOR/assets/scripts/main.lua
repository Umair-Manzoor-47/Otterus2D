math.randomseed(os.time())

run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/tilemap/test_map.lua")
run_script("assets/scripts/utilities.lua")
run_script("assets/scripts/rain_generator.lua")
run_script("assets/scripts/physics_showcase.lua")
run_script("assets/scripts/animal_wander.lua")

local tilemap = CreateTestMap()
assert(tilemap)
LoadAssets(AssetDefs)
LoadMap(tilemap)
Music.play("bgm")

local bunny1 = LoadEntity(AnimalsDefs.baby_bunny)
local bunny2 = LoadEntity(AnimalsDefs.baby_bunny_grey)

local animalManager = AnimalWanderManager:Create()
animalManager:Add(bunny1, {
    speed = 35.0,
    radius = 60,
    wait_min = 1.0,
    wait_max = 3.0,
    flip_sprite = true
})
animalManager:Add(bunny2, {
    speed = 40.0,
    radius = 70,
    wait_min = 0.5,
    wait_max = 2.5,
    flip_sprite = true
})

local rainGen = RainGenerator:Create()

gTimer = Timer()
gTimer:start()
local bDegenerate = false

function GetRandomValue(min, max)
	math.randomseed(get_ticks())
	return math.random(min, max)
end

main = {
    [1] = {
        update = function()
            rainGen:Update(0.016)
            animalManager:Update(0.016)
        end
    },

    [2] = {
        render = function()
        end
    }
}
