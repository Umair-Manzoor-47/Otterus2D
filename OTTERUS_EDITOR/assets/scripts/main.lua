math.randomseed(os.time())

run_script("assets/scripts/asteroids/entityDefs.lua")
run_script("assets/scripts/asteroids/assetDef.lua")
run_script("assets/scripts/asteroids/utilities.lua")
run_script("assets/scripts/asteroids/ship.lua")
run_script("assets/scripts/asteroids/asteroid.lua")
run_script("assets/scripts/asteroids/projectile.lua")
run_script("assets/scripts/asteroids/collision_system.lua")
run_script("assets/scripts/asteroids/game_data.lua")

LoadAssets()
LoadBackground()

local ship = LoadEntity(ShipDefs["red_ship"])

gShip = Ship:Create({id = ship})
gCollision = CollisionSystem:Create()

Music.play("bgm")

main = {
	[1] = {
		update = function()
			gShip:UpdateShip()
			SpawnAsteroids()
			UpdateAsteroids()
			UpdateProjectiles()
			gCollision:Update()
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
	},
	[2] = {
		render = function()
		end
	}
}