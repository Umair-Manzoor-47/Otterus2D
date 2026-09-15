math.randomseed(os.time())

run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/tilemap/test_map.lua")
run_script("assets/scripts/utilities.lua")
run_script("assets/scripts/physics_showcase.lua")

--local tilemap = CreateTestMap()
--assert(tilemap)
LoadAssets(AssetDefs)
--LoadMap(tilemap)
--Music.play("bgm")
--LoadEntity(AnimalsDefs.baby_bunny)
--LoadEntity(AnimalsDefs.baby_bunny_grey)

-- Initialize Physics / Ball Showcase
PhysicsShowcase:init()

main = {
    [1] = {
        update = function()
            PhysicsShowcase:update()
        end
    },

    [2] = {
        render = function()
            PhysicsShowcase:render()
        end
    }
}
