-- Main Lua Script!

run_script("assets/scripts/asteroids/entityDefs.lua")
run_script("assets/scripts/asteroids/assetDef.lua")
run_script("assets/scripts/asteroids/utilities.lua")

LoadAssets()

local ship = LoadEntity(ShipDefs["red_ship"])
local ast1 = LoadEntity(AsteroidDefs["asteroid_big"])
local ast2 = LoadEntity(AsteroidDefs["asteroid_small"])

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