run_script("../Games/asteroids/entityDefs.lua")
run_script("../Games/asteroids/assetDef.lua")
run_script("../Games/asteroids/utilities.lua")
run_script("../Games/asteroids/ship.lua")
run_script("../Games/asteroids/asteroid.lua")
run_script("../Games/asteroids/projectile.lua")
run_script("../Games/asteroids/collision_system.lua")
run_script("../Games/asteroids/game_data.lua")


SpaceShooter = {}

function SpaceShooter:Init()
    LoadAssets()
    LoadBackground()

    local ship = LoadEntity(ShipDefs["red_ship"])

    self.ship = Ship:Create({
        id = ship
    })

    self.collision = CollisionSystem:Create()

    Music.play("bgm")
end


function SpaceShooter:Update()
    self.ship:UpdateShip()

    SpawnAsteroids()
    UpdateAsteroids()
    UpdateProjectiles()

    self.collision:Update()

    if Keyboard.just_pressed(KEY_Z) then
        Music.pause()
    end

    if Keyboard.just_pressed(KEY_C) then
        Music.stop()
    end

    if Keyboard.just_pressed(KEY_X) then
        Music.resume()
    end
end


function SpaceShooter:Render()
    -- Asteroids rendering logic
end


SpaceShooter:Init()