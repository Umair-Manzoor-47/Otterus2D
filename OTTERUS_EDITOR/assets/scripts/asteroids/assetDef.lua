AssetDefs = {

	textures = {
	
		{ name = "ship", path = "assets/textures/aestroid/ship.png", pixel_art = true },
		{ name = "ast1", path = "assets/textures/aestroid/meteor_small.png", pixel_art = true },
		{ name = "ast2", path = "assets/textures/aestroid/meteor_big.png", pixel_art = true },
		{ name = "bg", path = "assets/textures/aestroid/bg.png", pixel_art = true },
	
	}

}

function LoadAssets()
	for k, v in pairs(AssetDefs.textures) do
		if not AssetManager.add_texture(v.name, v.path, v.pixel_art) then
			print("Failed to load texture [" ..v.name .."] at path [" ..v.path .."]")
		else
			print("Loaded texture [" ..v.name .."]")
		end
	end


end
