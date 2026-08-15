math.randomseed(os.time())

run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/tilemap/test_map.lua")
run_script("assets/scripts/utilities.lua")



local tilemap = CreateTestMap()
assert(tilemap)
LoadAssets(AssetDefs)
LoadMap(tilemap)
Music.play("bgm")
main = {
    [1] = {
        update = function()

        end
    },

    [2] = {
        render = function()
        end
    }
}