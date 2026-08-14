AssetDefs = {

	textures = {
	
		{ name = "ship", path = "../Games/asteroids/textures/ship.png", pixel_art = true },
		{ name = "ast1", path = "../Games/asteroids/textures/meteor_small.png", pixel_art = true },
		{ name = "ast2", path = "../Games/asteroids/textures/meteor_big.png", pixel_art = true },
		{ name = "bg", path = "../Games/asteroids/textures/bg.png", pixel_art = true },
		{ name = "laser", path = "../Games/asteroids/textures/laser.png", pixel_art = true },
	
	},

	music = {{ name = "bgm", path = "../Games/asteroids/music/bgm.mp3" }},
	sfx = {{ name = "laser", path = "../Games/asteroids/music/laser.mp3" }}

}

function LoadAssets()
	for k, v in pairs(AssetDefs.textures) do
		if not AssetManager.add_texture(v.name, v.path, v.pixel_art) then
			print("Failed to load texture [" ..v.name .."] at path [" ..v.path .."]")
		else
			print("Loaded texture [" ..v.name .."]")
		end
	end

	for k, v in pairs(AssetDefs.music) do
		if not AssetManager.add_music(v.name, v.path) then
			print("Failed to add music [" ..v.name .."] at path [" ..v.path .."]")
		else
			print("Added music [" ..v.name .."]")
		end
	end

	for k, v in pairs(AssetDefs.sfx) do
		if not AssetManager.add_sound(v.name, v.path) then
			print("Failed to add sfx [" ..v.name .."] at path [" ..v.path .."]")
		else
			print("Added sfx [" ..v.name .."]")
		end
	end

end
