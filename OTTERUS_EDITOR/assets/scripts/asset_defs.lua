AssetDefs = {

	textures = {
	
		{ name = "collider", path = "assets/textures/tilemap_test/collider.png", pixel_art = true },
		{ name = "ground_tiles", path = "assets/textures/tilemap_test/tiles/tiles.png", pixel_art = true },
		{ name = "bunny_baby", path = "assets/textures/tilemap_test/tiles/bunny_baby animation.png", pixel_art = true },
		{ name = "bunny_baby_grey", path = "assets/textures/tilemap_test/tiles/bunny_baby_grey animation.png", pixel_art = true },
		
		
	},

	music = {{ name = "bgm", path = "assets/music/bgm.mp3" }},
	sfx = {}

}


AnimalsDefs = {

	baby_bunny = {
		tag = "bunny",
		group = "animals",
		components = {
			transform = {
				position = { x = 175, y = 250 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "bunny_baby",
				width = 16,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 4
			},
			animation = {
				numFrames = 4, 
				frameRate = 5,
				frameOffset = 0,
				isVertical = false
			}
		
		}
	},
		baby_bunny_grey = {
		tag = "bunny_grey",
		group = "animals",
		components = {
			transform = {
				position = { x = 150, y = 300 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "bunny_baby_grey",
				width = 16,
				height = 16,
				start_x = 0, start_y = 2,
				layer = 4
			},
			animation = {
				numFrames = 4, 
				frameRate = 5,
				frameOffset = 0,
				isVertical = false
			}
		
		}
	}

}