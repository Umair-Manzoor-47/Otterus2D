AsteroidDefs = {
	asteroid_big = {
		group = "asteroids",
		components = {
			transform = {
				position = { x = 50, y = 100 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "ast2",
				width = 101,
				height = 84,
				start_x = 0, start_y = 0,
				layer = 2
			}
		}

	},
	asteroid_small = {
		group = "asteroids",
		components = {
			transform = {
				position = { x = 200, y = 200 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "ast1",
				width = 43,
				height = 43,
				start_x = 0, start_y = 0,
				layer = 2
			}
		}

	}

}

ShipDefs = {
	
	 red_ship = {
		tag = "ship",
		components = {
			transform = {
				position = { x = 320, y = 240 },
				scale = { x = 1, y = 1 },
				rotation = 0
			},
			sprite = {
				asset_name = "ship",
				width = 112,
				height = 75,
				start_x = 0, start_y = 0,
				layer = 2
			}
		}
	
	
	}

}