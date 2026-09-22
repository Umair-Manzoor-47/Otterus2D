-- =============================================================================
--  Cozy Scene Showcase
--  Tilemap, wandering bunnies, and rain generator
--  Applied Principles: KISS, DRY, YAGNI
-- =============================================================================

run_script("assets/scripts/CozySceneShowcase/test_map.lua")
run_script("assets/scripts/CozySceneShowcase/rain_generator.lua")
run_script("assets/scripts/CozySceneShowcase/animal_wander.lua")

CozySceneShowcase = {}
CozySceneShowcase.__index = CozySceneShowcase

-- Bunny configuration data (DRY: unified spawn list)
local TOP_RIGHT_BOUNDS = { min_x = 440, max_x = 600, min_y = 32, max_y = 160, speed = 35.0, wait_min = 0.5, wait_max = 2.5 }

local BUNNY_SPAWNS = {
    { def = AnimalsDefs.baby_bunny,      x = 175, y = 250, bounds = { radius = 60, speed = 35.0, wait_min = 1.0, wait_max = 3.0 } },
    { def = AnimalsDefs.baby_bunny_grey, x = 150, y = 300, bounds = { radius = 70, speed = 40.0, wait_min = 0.5, wait_max = 2.5 } },
    { def = AnimalsDefs.baby_bunny,      x = 520, y = 48,  bounds = TOP_RIGHT_BOUNDS },
    { def = AnimalsDefs.baby_bunny_grey, x = 570, y = 96,  bounds = TOP_RIGHT_BOUNDS },
    { def = AnimalsDefs.baby_bunny,      x = 480, y = 130, bounds = TOP_RIGHT_BOUNDS },
}

function CozySceneShowcase:init()
    local tilemap = CreateTestMap()
    assert(tilemap, "Failed to create tilemap")
    LoadMap(tilemap)
    -- Music.play("bgm")

    -- Spawn and register all bunnies
    self.animalManager = AnimalWanderManager:Create()
    for _, config in ipairs(BUNNY_SPAWNS) do
        local bunny = SpawnAnimal(config.def, config.x, config.y)
        self.animalManager:Add(bunny, config.bounds)
    end

    -- Rain particle generator
    self.rainGen = RainGenerator:Create()
end

function CozySceneShowcase:update(dt)
    dt = dt or 0.016
    if self.rainGen then self.rainGen:Update(dt) end
    if self.animalManager then self.animalManager:Update(dt) end
end

function CozySceneShowcase:Create()
    local this = setmetatable({}, CozySceneShowcase)
    this:init()
    return this
end
