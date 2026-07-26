
function LoadEntity(def)
	assert(def, "Def does not exists.")	
	
	local tag = ""
	if def.tag then
		tag = def.tag
	end

	local group = ""
	if def.group then
		group = def.group
	end

	local newEntity = Entity(tag, group)

	if def.components.transform then
		newEntity:add_component(
			Transform(
				vec2(
					def.components.transform.position.x,
					def.components.transform.position.y
				),
				vec2(
					def.components.transform.scale.x,
					def.components.transform.scale.y
				),
				def.components.transform.rotation
			)
		)
	end

	if def.components.sprite then
		local sprite = newEntity:add_component(
			Sprite(
				def.components.sprite.asset_name,
				def.components.sprite.width,
				def.components.sprite.height,
				def.components.sprite.start_x,
				def.components.sprite.start_y,
				def.components.sprite.layer
			)
		)
		sprite:generate_uvs()
	end

	return newEntity:id()

end


SCREEN_WIDTH = 640
SCREEN_HEIGHT = 480

-- Position vec2, widh/height = float
function CheckPosition(position, width, height)
	local min_x = 0
	local min_y = 0
	local max_x = SCREEN_WIDTH
	local max_y = SCREEN_HEIGHT

	if position.x + width < min_x then
		position.x = position.x + SCREEN_WIDTH + width
	elseif position.x > max_x + width then
		position.x = -width
	end

	if position.y + height < min_y then
		position.y = position.y + SCREEN_HEIGHT + height
	elseif position.y > max_y + height then
		position.y = -height
	end

end

