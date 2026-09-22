math.randomseed(os.time())

-- Core Assets & Utilities
run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/utilities.lua")
LoadAssets(AssetDefs)

-- =============================================================================
-- Active Scene: CozySceneShowcase (Tilemap, wandering bunnies, rain generator)
-- =============================================================================
--run_script("assets/scripts/CozySceneShowcase/cozy_scene.lua")
--local cozyScene = CozySceneShowcase:Create()

-- =============================================================================
-- Standby Scene: PlatformerController (Uncomment to switch back to platformer)
-- =============================================================================
run_script("assets/scripts/platformer_controller.lua")
local platformer = PlatformerController:Create()

main = {
    [1] = {
        update = function()
            -- cozyScene:update(0.016)
            platformer:update(0.016)
        end
    },

    [2] = {
        render = function()
            platformer:render()
        end
    }
}
