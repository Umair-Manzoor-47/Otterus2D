-- State machine test

run_script("assets/scripts/state_machine_test/game_state.lua")
run_script("assets/scripts/state_machine_test/title_state.lua")

StateMachineTest = {}
StateMachineTest.__index = StateMachineTest

function StateMachineTest:Create()
	local this = setmetatable({}, StateMachineTest)
	this.m_StateStack = StateStack()
	gStateStack = this.m_StateStack

	local title = TitleState:Create(this.m_StateStack)
	this.m_StateStack:change_state(title)
	return this
end

function StateMachineTest:update(dt)
	dt = dt or 0.15
	if self.m_StateStack then
		self.m_StateStack:update(dt)
	end
end

function StateMachineTest:render()
	if self.m_StateStack then
		self.m_StateStack:render()
	end
end
