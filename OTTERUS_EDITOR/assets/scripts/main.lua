math.randomseed(os.time())

run_script("../Games/asteroids/game.lua")

main = {
    [1] = {
        update = function()
            SpaceShooter:Update()
        end
    },

    [2] = {
        render = function()
            SpaceShooter:Render()
        end
    }
}