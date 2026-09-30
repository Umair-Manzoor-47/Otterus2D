math.randomseed(os.time())

-- Core Assets & Utilities
run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/utilities.lua")
run_script("assets/scripts/events/event_manager.lua")
run_script("assets/scripts/events/collision_event.lua")
run_script("assets/scripts/systems/trigger_system.lua")
LoadAssets(AssetDefs)

-- =============================================================================
-- Logger Tests: Verify all 3 logging types (INFO, WARN, ERROR) & OTLog alias
-- =============================================================================
OT_log("=== Logger Test: INFO Log ===")
OT_log("INFO format test: EntityID=%d, Name='%s', Pos=(%.1f, %.1f)", 1, "Player", 16.0, 416.0)

OT_warn("=== Logger Test: WARN Log ===")
OT_warn("WARN format test: Asset [%s] took %.2f ms to load", "NinjaFrog", 14.50)

OT_error("=== Logger Test: ERROR Log ===")
OT_error("ERROR format test: Code %d - Failed to open resource at '%s'", 404, "assets/textures/missing.png")

-- Testing OTLog alias (supports both OTLog("...") and OTLog.warn/error)
if OTLog then
    OTLog("=== Logger Test: OTLog callable alias (INFO) ===")
    if OTLog.warn then OTLog.warn("=== Logger Test: OTLog.warn table method ===") end
    if OTLog.error then OTLog.error("=== Logger Test: OTLog.error table method ===") end
end


-- =============================================================================
-- Active Scene: CozySceneShowcase (Tilemap, wandering bunnies, rain generator)
-- =============================================================================
--run_script("assets/scripts/CozySceneShowcase/cozy_scene.lua")
--local cozyScene = CozySceneShowcase:Create()

-- =============================================================================
-- Standby Scene: PlatformerController
-- =============================================================================
run_script("assets/scripts/Platformer/platformer_controller.lua")
local platformer = PlatformerController:Create()

-- =============================================================================
-- Event System Initialization
-- =============================================================================
local collisionEvent = CollisionEvent:Create()
local triggerSystem  = TriggerSystem:Create()
collisionEvent:SubscribeToEvent(triggerSystem)

main = {
    [1] = {
        update = function()
           -- cozyScene:update(0.016)
           platformer:update(0.016)

           local uda, udb = ContactListener.get_user_data()
           if uda and udb then
               collisionEvent:EmitEvent(uda, udb)
           end
        end
    },

    [2] = {
        render = function()
           platformer:render()
        end
    }
}
