math.randomseed(os.time())

run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/tilemap/test_map.lua")
run_script("assets/scripts/utilities.lua")


AssetManager.add_font("Minecraft", "assets/fonts/Minecraft.ttf", 32.0)


local tilemap = CreateTestMap()
assert(tilemap)
LoadAssets(AssetDefs)
LoadMap(tilemap)
Music.play("bgm")
LoadEntity(AnimalsDefs.baby_bunny)
LoadEntity(AnimalsDefs.baby_bunny_grey)
DrawLine(Line(vec2(50, 50), vec2(200, 200), Color(255, 0, 0, 255)))
DrawLine(Line(vec2(200, 50), vec2(50, 200), Color(0, 0, 255, 255)))
DrawRect(Rect(vec2(300, 300), 100, 100, Color(255, 0, 0, 255)))
DrawText(Text(vec2(100.0, 100.0), "Grandfall Claudi, The Black Dragon.", "Minecraft", 320.0, Color(255, 255, 0, 255)))

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