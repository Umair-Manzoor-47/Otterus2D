-- Game state

GameState = {}
GameState.__index = GameState

function GameState:Create(stack)
	local this = 
	{
		m_Stack = stack 
	}
	
	local state = State("game state")
	state:set_variable_table(this)
	state:set_on_enter(
		function()
			print("Enter Game State")
		end
	)
	state:set_on_exit(
		function()
			print("Exit Game State")
		end
	)

	state:set_on_update(
		function(dt)
			print("Update GameState")
		end
	)

	state:set_on_render(
		function()
			print("Render Game State")
		end
	)

	state:set_handle_inputs(
		function()
			this:HandleInputs()
		end
	)

	setmetatable(this, self)
	return state
end

function GameState:HandleInputs()
	if Keyboard.just_released(KEY_BACKSPACE) then 
		self.m_Stack:pop()
		return 
	end
end
