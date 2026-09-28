CollisionEvent = Event:Create() 

function CollisionEvent:Create()
	local this = CreateObject(CollisionEvent):new({
		m_Subscribers = {}
	})
	local object_a = nil 
	local object_b = nil 

	function this:Execute()
		for k, v in pairs(self.m_Subscribers) do 
			v:OnCollision(object_a, object_b)
		end
	end

	function this:EmitEvent(obj_a, obj_b)
		object_a = obj_a 
		object_b = obj_b 
		self:Execute()
	end

	return this
end