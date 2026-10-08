math.randomseed(os.time())

run_script("assets/scripts/script_list.lua")
O2D_load_script_table(script_list)
LoadAssets(AssetDefs)

local stateStack = StateStack()
gStateStack = stateStack

local titleState = PlatformerTitleState:Create(stateStack, nil)
stateStack:change_state(titleState)

main = {
    [1] = {
        update = function()
            stateStack:update(0.016)
        end
    },

    [2] = {
        render = function()
            stateStack:render()
        end
    }
}
