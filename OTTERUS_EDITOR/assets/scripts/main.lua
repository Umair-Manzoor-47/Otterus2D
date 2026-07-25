-- Main Lua Script!

if run_script("assets/scripts/asteroids/entityDefs.lua") then
	print("script registered")
end

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