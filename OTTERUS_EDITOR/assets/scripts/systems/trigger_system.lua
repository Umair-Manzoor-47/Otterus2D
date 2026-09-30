TriggerSystem = Subscriber:Create()

function TriggerSystem:Create()
	return CreateObject(TriggerSystem):new()
end 

function TriggerSystem:OnCollision(object_a, object_b)
	local isTriggerA = object_a.isTrigger
	local isPlayerA  = object_a.group == "player" or object_a.tag == "player" or object_a.tag == "NinjaFrog"

	local isTriggerB = object_b.isTrigger
	local isPlayerB  = object_b.group == "player" or object_b.tag == "player" or object_b.tag == "NinjaFrog"

	if isTriggerA and isPlayerB then 
		self:OnPlayerTriggered(object_a, object_b)
	elseif isTriggerB and isPlayerA then 
		self:OnPlayerTriggered(object_b, object_a)
	end 
end

function TriggerSystem:OnPlayerTriggered(trigger, player)
	OT_log("Trigger [%s] (%s) has been activated by [%s]!", trigger.tag, trigger.group, player.tag)
	local id = player.entityId or player.entityID
	local playerEntity = Entity(id)
	local physics = playerEntity:get_component(PhysicsComponent)
	physics:set_transform(vec2(16, 416))
	physics:linear_impulse(vec2(0, 5))
end