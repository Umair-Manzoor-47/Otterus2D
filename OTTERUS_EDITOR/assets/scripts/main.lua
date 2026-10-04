math.randomseed(os.time())

-- Core Assets & Utilities
run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/utilities.lua")
run_script("assets/scripts/events/event_manager.lua")
run_script("assets/scripts/events/collision_event.lua")
run_script("assets/scripts/systems/trigger_system.lua")
LoadAssets(AssetDefs)


-- Active Scene
run_script("assets/scripts/CozySceneShowcase/cozy_scene.lua")
local cozyScene = CozySceneShowcase:Create()

-- Standby: PlatformerController
--run_script("assets/scripts/Platformer/platformer_controller.lua")
--local platformer = PlatformerController:Create()

-- Event System
--local collisionEvent = CollisionEvent:Create()
--local triggerSystem  = TriggerSystem:Create()
--collisionEvent:SubscribeToEvent(triggerSystem)

-- Standby: StateMachineTest
--run_script("assets/scripts/state_machine_test/state_machine_test.lua")
--local stateMachineTest = StateMachineTest:Create()


main = {
    [1] = {
        update = function()
           cozyScene:update(0.016)
           -- platformer:update(0.016)
           -- 
           -- local uda, udb = ContactListener.get_user_data()
           -- if uda and udb then
           --     collisionEvent:EmitEvent(uda, udb)
           -- end
           -- stateMachineTest:update(0.15)
        end
    },

    [2] = {
        render = function()
           -- platformer:render()
           -- stateMachineTest:render()
        end
    }
}
