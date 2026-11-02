AssetDefs = {

	textures = {
	
		{ name = "ship", path = "../Games/asteroids/textures/ship.png", pixel_art = true, tileset = false },
		{ name = "ast1", path = "../Games/asteroids/textures/meteor_small.png", pixel_art = true, tileset = false },
		{ name = "ast2", path = "../Games/asteroids/textures/meteor_big.png", pixel_art = true, tileset = false },
		{ name = "bg", path = "../Games/asteroids/textures/bg.png", pixel_art = true, tileset = false },
		{ name = "laser", path = "../Games/asteroids/textures/laser.png", pixel_art = true, tileset = false },
	
	},

	music = {{ name = "bgm", path = "../Games/asteroids/music/bgm.mp3" }},
	sfx = {{ name = "laser", path = "../Games/asteroids/music/laser.mp3" }}

}

function LoadAssets()
	for k, v in pairs(AssetDefs.textures) do
		local tileset = v.tileset or false
		if not AssetManager.add_texture(v.name, v.path, v.pixel_art, tileset) then
			OT_error("Failed to load texture [%s] at path [%s]", v.name, v.path)
		else
			OT_log("Loaded texture [%s]", v.name)
		end
	end

	for k, v in pairs(AssetDefs.music) do
		if not AssetManager.add_music(v.name, v.path) then
			OT_error("Failed to add music [%s] at path [%s]", v.name, v.path)
		else
			OT_log("Added music [%s]", v.name)
		end
	end

	for k, v in pairs(AssetDefs.sfx) do
		if not AssetManager.add_sound(v.name, v.path) then
			OT_error("Failed to add sfx [%s] at path [%s]", v.name, v.path)
		else
			OT_log("Added sfx [%s]", v.name)
		end
	end

end
