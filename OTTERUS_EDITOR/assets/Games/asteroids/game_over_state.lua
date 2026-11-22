AsteroidsGameOverState = {}
AsteroidsGameOverState.__index = AsteroidsGameOverState

function AsteroidsGameOverState:Create(stack)
    local this = {
        m_Stack = stack,
        m_Entities = {}
    }

    local state = State("asteroids_game_over_state")
    state:set_variable_table(this)

    state:set_on_enter(function()
        this:OnEnter()
    end)

    state:set_on_exit(function()
        this:OnExit()
    end)

    state:set_on_update(function(dt)
        this:OnUpdate(dt)
    end)

    state:set_handle_inputs(function()
        this:HandleInputs()
    end)

    setmetatable(this, self)
    return state
end

function AsteroidsGameOverState:OnEnter()
    Camera.get().set_position(vec2(0, 0))
    Camera.get().set_scale(1)

    LoadBackground()

    local titleEnt = Entity("GameOver_Title", "UI")
    titleEnt:add_component(Transform(vec2(220, 160), vec2(1, 1), 0))
    titleEnt:add_component(TextComponent("GAME OVER", "pixel", 0, -1.0, Color(255, 60, 60, 255)))
    table.insert(self.m_Entities, titleEnt)

    local scoreEnt = Entity("Final_Score", "UI")
    scoreEnt:add_component(Transform(vec2(210, 220), vec2(1, 1), 0))
    scoreEnt:add_component(TextComponent("Final Score: " .. tostring(gData:GetScore()), "pixel", 0, -1.0, Color(255, 255, 255, 255)))
    table.insert(self.m_Entities, scoreEnt)

    local retryEnt = Entity("Retry_Prompt", "UI")
    retryEnt:add_component(Transform(vec2(190, 270), vec2(1, 1), 0))
    retryEnt:add_component(TextComponent("[Enter] Play Again", "pixel", 0, -1.0, Color(255, 220, 0, 255)))
    table.insert(self.m_Entities, retryEnt)

    local menuEnt = Entity("Menu_Prompt", "UI")
    menuEnt:add_component(Transform(vec2(190, 320), vec2(1, 1), 0))
    menuEnt:add_component(TextComponent("[Esc] Return to Title", "pixel", 0, -1.0, Color(160, 160, 160, 255)))
    table.insert(self.m_Entities, menuEnt)
end

function AsteroidsGameOverState:OnExit()
    for _, ent in ipairs(self.m_Entities) do
        ent:kill()
    end
    self.m_Entities = {}
    ClearBackground()
end

function AsteroidsGameOverState:OnUpdate(dt)
end

function AsteroidsGameOverState:HandleInputs()
    if Keyboard.just_released(KEY_ENTER) or Keyboard.just_pressed(KEY_ENTER)
    or Keyboard.just_released(KEY_SPACE) or Keyboard.just_pressed(KEY_SPACE) then
        self.m_Stack:change_state(AsteroidsGameState:Create(self.m_Stack))
    elseif Keyboard.just_released(KEY_ESC) or Keyboard.just_released(KEY_BACKSPACE) then
        self.m_Stack:change_state(AsteroidsTitleState:Create(self.m_Stack))
    end
end
