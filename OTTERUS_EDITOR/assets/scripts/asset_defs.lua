AssetDefs = {

	textures = {
	
		-- Cozy Scene
		{ name = "ground_tiles", path = "assets/Games/CozyScene/textures/tiles/tiles.png", pixel_art = true, tileset = true },
		{ name = "bunny_baby", path = "assets/Games/CozyScene/textures/tiles/bunny_baby animation.png", pixel_art = true },
		{ name = "bunny_baby_grey", path = "assets/Games/CozyScene/textures/tiles/bunny_baby_grey animation.png", pixel_art = true },
		{ name = "chicken_baby", path = "assets/Games/CozyScene/textures/tiles/chicken_baby animation.png", pixel_art = true },
		{ name = "chicken_brown", path = "assets/Games/CozyScene/textures/tiles/chicken_brown animation.png", pixel_art = true },
		{ name = "cow", path = "assets/Games/CozyScene/textures/tiles/cow animation.png", pixel_art = true },
		{ name = "rain", path = "assets/Games/CozyScene/textures/rain.png", pixel_art = true},
		{ name = "otterus2d_logo", path = "assets/Games/CozyScene/textures/tiles/otterus2D_word.png", pixel_art = true },
		{ name = "title_panel", path = "assets/Games/CozyScene/textures/tiles/panel.png", pixel_art = true },
		{ name = "leaf_particles", path = "assets/Games/CozyScene/textures/tiles/leaf_particles.png", pixel_art = true },
		{ name = "white_flash", path = "assets/Games/CozyScene/textures/white_flash.png", pixel_art = true },
		{ name = "font_O", path = "assets/Games/CozyScene/textures/tiles/font/letter_O_upper.png", pixel_art = true },
		{ name = "font_t", path = "assets/Games/CozyScene/textures/tiles/font/letter_t.png", pixel_art = true },
		{ name = "font_e", path = "assets/Games/CozyScene/textures/tiles/font/letter_e.png", pixel_art = true },
		{ name = "font_r", path = "assets/Games/CozyScene/textures/tiles/font/letter_r.png", pixel_art = true },
		{ name = "font_u", path = "assets/Games/CozyScene/textures/tiles/font/letter_u.png", pixel_art = true },
		{ name = "font_s", path = "assets/Games/CozyScene/textures/tiles/font/letter_s.png", pixel_art = true },
		{ name = "font_2", path = "assets/Games/CozyScene/textures/tiles/font/letter_2.png", pixel_art = true },
		{ name = "font_D", path = "assets/Games/CozyScene/textures/tiles/font/letter_D_upper.png", pixel_art = true },

		-- Platformer Scene
		{ name = "nf_idle", path = "assets/Games/Platformer/textures/Players/Ninja Frog/Idle (32x32).png",  pixel_art = true },
		{ name = "nf_run",  path = "assets/Games/Platformer/textures/Players/Ninja Frog/Run (32x32).png",   pixel_art = true },
		{ name = "nf_jump", path = "assets/Games/Platformer/textures/Players/Ninja Frog/Jump (32x32).png",  pixel_art = true },
		{ name = "nf_double_jump", path = "assets/Games/Platformer/textures/Players/Ninja Frog/Double Jump (32x32).png", pixel_art = true },
		{ name = "nf_fall", path = "assets/Games/Platformer/textures/Players/Ninja Frog/Fall (32x32).png",  pixel_art = true },
		{ name = "nf_hit",  path = "assets/Games/Platformer/textures/Players/Ninja Frog/Hit (32x32).png",   pixel_art = true },
		{ name = "nf_wall_jump", path = "assets/Games/Platformer/textures/Players/Ninja Frog/Wall Jump (32x32).png", pixel_art = true },
		{ name = "Terrain", path = "assets/Games/Platformer/textures/Terrain/Terrain (16x16).png", pixel_art = true, tileset = true },

		-- Asteroids Scene
		{ name = "ship", path = "assets/Games/asteroids/textures/ship.png", pixel_art = true },
		{ name = "ast1", path = "assets/Games/asteroids/textures/meteor_small.png", pixel_art = true },
		{ name = "ast2", path = "assets/Games/asteroids/textures/meteor_big.png", pixel_art = true },
		{ name = "bg", path = "assets/Games/asteroids/textures/bg.png", pixel_art = true },
		{ name = "laser", path = "assets/Games/asteroids/textures/laser.png", pixel_art = true },

		-- Shared Assets
		{ name = "ball", path = "assets/textures/shared/ball.png", pixel_art = true },
	},

	music = {
		{ name = "bgm", path = "assets/music/bgm.mp3" },
		{ name = "asteroids_bgm", path = "assets/Games/asteroids/music/bgm.mp3" },
	},
	sfx = {
		{ name = "thunder", path = "assets/sfx/thunder.wav" },
		{ name = "leaves_rustle", path = "assets/sfx/leaves_rustle.wav" },
		{ name = "laser", path = "assets/sfx/laser.wav" },
		{ name = "asteroids_laser", path = "assets/Games/asteroids/music/laser.mp3" },
	},
	fonts = {{name = "Minecraft", path = "assets/fonts/Minecraft.ttf", size = 32.0}}

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
				layer = 1
			},
			animation = {
				numFrames = 4, 
				frameRate = 5,
				frameOffset = 0,
				isVertical = false,
				looped = true
			},
			box_collider = {
				width = 16,
				height = 16,
				offset = { x = 0, y = 0 }
			},
			physics = {
				type = BodyType.DYNAMIC,
				density = 10.0,
				friction = 0.9,
				restitution = 0.0,
				gravityScale = 0.0,
				fixedRotation = true,
				isFriendly = true
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
				start_x = 0, start_y = 0,
				layer = 1
			},
			animation = {
				numFrames = 4, 
				frameRate = 5,
				frameOffset = 0,
				isVertical = false,
				looped = true
			},
			box_collider = {
				width = 16,
				height = 16,
				offset = { x = 0, y = 0 }
			},
			physics = {
				type = BodyType.DYNAMIC,
				density = 10.0,
				friction = 0.9,
				restitution = 0.0,
				gravityScale = 0.0,
				fixedRotation = true,
				isFriendly = true
			}
		}
	},
	chicken_baby = {
		tag = "chicken_baby",
		group = "animals",
		components = {
			transform = {
				position = { x = 110, y = 180 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "chicken_baby",
				width = 16,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 1
			},
			animation = {
				numFrames = 4, 
				frameRate = 5,
				frameOffset = 0,
				isVertical = false,
				looped = true
			},
			box_collider = {
				width = 16,
				height = 16,
				offset = { x = 0, y = 0 }
			},
			physics = {
				type = BodyType.DYNAMIC,
				density = 10.0,
				friction = 0.9,
				restitution = 0.0,
				gravityScale = 0.0,
				fixedRotation = true,
				isFriendly = true
			}
		}
	},
	chicken_brown = {
		tag = "chicken_brown",
		group = "animals",
		components = {
			transform = {
				position = { x = 150, y = 190 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "chicken_brown",
				width = 16,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 1
			},
			animation = {
				numFrames = 4, 
				frameRate = 5,
				frameOffset = 0,
				isVertical = false,
				looped = true
			},
			box_collider = {
				width = 16,
				height = 16,
				offset = { x = 0, y = 0 }
			},
			physics = {
				type = BodyType.DYNAMIC,
				density = 10.0,
				friction = 0.9,
				restitution = 0.0,
				gravityScale = 0.0,
				fixedRotation = true,
				isFriendly = true
			}
		}
	},
	cow = {
		tag = "cow",
		group = "animals",
		components = {
			transform = {
				position = { x = 260, y = 390 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "cow",
				width = 24,
				height = 24,
				start_x = 0, start_y = 0,
				layer = 1
			},
			animation = {
				numFrames = 4, 
				frameRate = 4,
				frameOffset = 0,
				isVertical = false,
				looped = true
			},
			box_collider = {
				width = 24,
				height = 24,
				offset = { x = 0, y = 0 }
			},
			physics = {
				type = BodyType.DYNAMIC,
				density = 25.0,
				friction = 0.9,
				restitution = 0.0,
				gravityScale = 0.0,
				fixedRotation = true,
				isFriendly = true
			}
		}
	}

}