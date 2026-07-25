AsteroidDefs = {
	asteroid_big = {
		group = "asteroids",
		components = {
			transform = {
				position = { x = -100, y = -100 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "ast1",
				width = 112,
				height = 80,
				start_x = 0, start_y = 0,
				layer = 2
			}
		}

	},
	asteroid_small = {
		group = "asteroids",
		components = {
			transform = {
				position = { x = -100, y = -100 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "ast2",
				width = 32,
				height = 32,
				start_x = 0, start_y = 0,
				layer = 2
			}
		}

	}

}

ShipDefs = {
	
	blue_ship = {
		tag = "ship",
		components = {
			transform = {
				position = { x = -100, y = -100 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "ship",
				width = 96,
				height = 80,
				start_x = 0, start_y = 0,
				layer = 2
			}
		}
	
	
	}

}