-- Cozy Scene Showcase

run_script("assets/scripts/CozySceneShowcase/test_map.lua")
run_script("assets/scripts/CozySceneShowcase/rain_generator.lua")
run_script("assets/scripts/CozySceneShowcase/animal_wander.lua")
run_script("assets/scripts/CozySceneShowcase/ambient_effects.lua")

CozySceneShowcase = {}
CozySceneShowcase.__index = CozySceneShowcase

local TOP_RIGHT_BOUNDS        = { min_x = 440, max_x = 600, min_y = 32,  max_y = 160, speed = 35.0, wait_min = 0.5, wait_max = 2.5, avoid_radius = 24.0 }
local BOTTOM_RIGHT_BOUNDS     = { min_x = 500, max_x = 600, min_y = 350, max_y = 430, speed = 38.0, wait_min = 0.8, wait_max = 2.5, avoid_radius = 24.0 }

-- East pasture (cows)
local EAST_PASTURE_COW_BOUNDS = { min_x = 415, max_x = 475, min_y = 200, max_y = 335, speed = 18.0, wait_min = 2.0, wait_max = 4.5, avoid_radius = 34.0 }

-- West clearing (chick)
local RED_CIRCLE_CHICK_BOUNDS = { min_x = 160, max_x = 230, min_y = 135, max_y = 195, speed = 25.0, wait_min = 0.8, wait_max = 2.5, avoid_radius = 20.0 }

-- Side pastures (chickens)
local WEST_SIDE_BOUNDS        = { min_x = 60,  max_x = 140, min_y = 240, max_y = 320, speed = 25.0, wait_min = 0.8, wait_max = 2.5, avoid_radius = 20.0 }
local SOUTH_WEST_BOUNDS       = { min_x = 90,  max_x = 180, min_y = 360, max_y = 430, speed = 24.0, wait_min = 1.0, wait_max = 3.0, avoid_radius = 20.0 }
local EAST_SIDE_BOUNDS        = { min_x = 530, max_x = 600, min_y = 210, max_y = 320, speed = 26.0, wait_min = 0.8, wait_max = 2.5, avoid_radius = 20.0 }

local ANIMAL_SPAWNS = {
    -- Cows
    { def = AnimalsDefs.cow,             x = 440, y = 240, bounds = EAST_PASTURE_COW_BOUNDS },
    { def = AnimalsDefs.cow,             x = 445, y = 300, bounds = EAST_PASTURE_COW_BOUNDS },

    -- Chick
    { def = AnimalsDefs.chicken_baby,    x = 195, y = 155, bounds = RED_CIRCLE_CHICK_BOUNDS },

    -- Chickens
    { def = AnimalsDefs.chicken_brown,   x = 95,  y = 280, bounds = WEST_SIDE_BOUNDS },
    { def = AnimalsDefs.chicken_baby,    x = 140, y = 390, bounds = SOUTH_WEST_BOUNDS },
    { def = AnimalsDefs.chicken_brown,   x = 560, y = 260, bounds = EAST_SIDE_BOUNDS },

    -- Bunnies
    { def = AnimalsDefs.baby_bunny,      x = 520, y = 48,  bounds = TOP_RIGHT_BOUNDS },
    { def = AnimalsDefs.baby_bunny_grey, x = 570, y = 96,  bounds = TOP_RIGHT_BOUNDS },
    { def = AnimalsDefs.baby_bunny,      x = 480, y = 130, bounds = TOP_RIGHT_BOUNDS },
    { def = AnimalsDefs.baby_bunny,      x = 175, y = 250, bounds = { radius = 60, speed = 35.0, wait_min = 1.0, wait_max = 3.0, avoid_radius = 24.0 } },
    { def = AnimalsDefs.baby_bunny_grey, x = 150, y = 300, bounds = { radius = 70, speed = 40.0, wait_min = 0.5, wait_max = 2.5, avoid_radius = 24.0 } },
    { def = AnimalsDefs.baby_bunny_grey, x = 550, y = 380, bounds = BOTTOM_RIGHT_BOUNDS },
}

function CozySceneShowcase:init()
    local tilemap = CreateTestMap()
    assert(tilemap, "Failed to create tilemap")
    LoadMap(tilemap)
    Music.play("bgm")

    -- Spawn animals
    self.animalManager = AnimalWanderManager:Create()
    for _, config in ipairs(ANIMAL_SPAWNS) do
        local animal = SpawnAnimal(config.def, config.x, config.y)
        self.animalManager:Add(animal, config.bounds)
    end

    self.rainGen = RainGenerator:Create()
    self.ambientEffects = AmbientEffectsManager:Create()

    -- Title banner
    self:SpawnTitleBanner(220, 92, 9, 10)
end

function CozySceneShowcase:SpawnTitleBanner(panelX, panelY, panelLayer, textLayer)
    panelLayer = panelLayer or 9
    textLayer  = textLayer  or 10

    -- Panel
    local panelEnt = Entity("otterus2d_panel", "ui")
    panelEnt:add_component(Transform(vec2(panelX, panelY), vec2(1, 1), 0))
    local panelSpr = panelEnt:add_component(Sprite("title_panel", 201, 98, 0, 0, panelLayer))
    panelSpr:generate_uvs()
    self.panelEntity = panelEnt

    -- Title text
    local textX = panelX + math.floor((201 - 172) / 2)
    local textY = panelY + math.floor((98 - 28) / 2)
    local titleEnt = Entity("otterus2d_title", "ui")
    titleEnt:add_component(Transform(vec2(textX, textY), vec2(1, 1), 0))
    local titleSpr = titleEnt:add_component(Sprite("otterus2d_logo", 172, 28, 0, 0, textLayer))
    titleSpr:generate_uvs()
    self.titleEntity = titleEnt

    return panelEnt, titleEnt
end

function CozySceneShowcase:SpawnTitleText(startX, startY, layer)
    return self:SpawnTitleBanner(startX - 14, startY - 35, (layer or 10) - 1, layer or 10)
end

function CozySceneShowcase:update(dt)
    dt = dt or 0.016

    -- Toggle collider debug with 'C' or 'F1'
    if Keyboard.just_pressed(KEY_C) or Keyboard.just_pressed(KEY_F1) then
        if IsRenderCollidersEnabled() then
            DisableRenderColliders()
        else
            EnableRenderColliders()
        end
    end

    if self.rainGen then self.rainGen:Update(dt) end
    if self.animalManager then self.animalManager:Update(dt) end
    if self.ambientEffects then self.ambientEffects:Update(dt) end
end

function CozySceneShowcase:Create()
    local this = setmetatable({}, CozySceneShowcase)
    this:init()
    return this
end
