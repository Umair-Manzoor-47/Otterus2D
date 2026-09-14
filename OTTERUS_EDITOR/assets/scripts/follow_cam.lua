FollowCam = {}
FollowCam.__index = FollowCam

function FollowCam:CreateCam( cam, params )
	local this = {
		m_Camera = params.cam or Camera.get(),
		m_Scale = params.scale or 2,
		m_MinX = params.min_x or 0,
		m_MinY = params.min_y or 0,
		m_MaxX = params.max_x * (params.scale or 2) or cam.width() * 2,
		m_MaxY = params.max_y * (params.scale or 2) or cam.height() * 2,
		m_SpringBack = params.spring_back or 1.0

	}

	this.m_Camera.set_position(vec2(this.m_MinX, this.m_MinY))
	this.m_Camera.set_scale(this.m_Scale)
	setmetatable(this, self)
	return this
end

function FollowCam:Update( entity_id )
	local entity = Entity( entity_id )
	local transform = entity:get_component(Transform)
	
	local cam_pos = self.m_Camera.position()
	local cam_width = self.m_Camera.width()
	local cam_height = self.m_Camera.height()

	--local new_cam_pos = vec2(cam_width / (2 * self.m_Scale), cam_height / (2 * self.m_Scale))
	local new_cam_pos = vec2(0, 0)

	new_cam_pos.x = (transform.position.x - cam_width / (2 * self.m_Scale)) * self.m_Scale
	new_cam_pos.y = (transform.position.y - cam_height / (2 * self.m_Scale)) * self.m_Scale

	-- clam camera position to min/max
	new_cam_pos.x = clamp(new_cam_pos.x, self.m_MinX, self.m_MaxX)
	new_cam_pos.y = clamp(new_cam_pos.y, self.m_MinY, self.m_MaxY)

	self.m_Camera.set_position(
		vec2(
			lerp(cam_pos.x, new_cam_pos.x, self.m_SpringBack),
			lerp(cam_pos.y, new_cam_pos.y, self.m_SpringBack)
		
		)
	)

end
