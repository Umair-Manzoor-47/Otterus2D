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
	print("Trigger [" .. trigger.tag .. "] (" .. trigger.group .. ") has been activated by [" .. player.tag .. "]!")
end