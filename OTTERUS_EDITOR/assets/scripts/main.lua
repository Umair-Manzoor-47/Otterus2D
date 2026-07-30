math.randomseed(os.time())

run_script("assets/scripts/asteroids/entityDefs.lua")
run_script("assets/scripts/asteroids/assetDef.lua")
run_script("assets/scripts/asteroids/utilities.lua")
run_script("assets/scripts/asteroids/ship.lua")
run_script("assets/scripts/asteroids/asteroid.lua")
run_script("assets/scripts/asteroids/projectile.lua")
run_script("assets/scripts/asteroids/collision_system.lua")

LoadAssets()
LoadBackground()

local ship = LoadEntity(ShipDefs["red_ship"])

gShip = Ship:Create({id = ship})
gCollision = CollisionSystem:Create()

main = {
	[1] = {
		update = function()
			gShip:UpdateShip()
			SpawnAsteroids()
			UpdateAsteroids()
			UpdateProjectiles()
			gCollision:Update()
		end
	},
	[2] = {
		render = function()
		end
	}
}