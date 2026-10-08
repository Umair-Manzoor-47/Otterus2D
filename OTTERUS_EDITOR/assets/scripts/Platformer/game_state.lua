-- Platformer Game State

PlatformerGameState = {}
PlatformerGameState.__index = PlatformerGameState

function PlatformerGameState:Create(stack, platformer)
    local this = {
        m_Stack = stack,
        m_Platformer = platformer
    }

    local state = State("platformer_game_state")
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

    state:set_on_render(function()
        this:OnRender()
    end)

    state:set_handle_inputs(function()
        this:HandleInputs()
    end)

    setmetatable(this, self)
    return state
end

function PlatformerGameState:OnEnter()
    if self.m_Platformer then
        self.m_Platformer:set_ui_visible(true)
    end
end

function PlatformerGameState:OnExit()
    if self.m_Platformer then
        self.m_Platformer:set_ui_visible(false)
    end
end

function PlatformerGameState:OnUpdate(dt)
    if self.m_Platformer then
        self.m_Platformer:update(dt)
    end
end

function PlatformerGameState:OnRender()
    if self.m_Platformer then
        self.m_Platformer:render()
    end
end

function PlatformerGameState:HandleInputs()
    -- Allow ESC or BACKSPACE to return to Title State
    if Keyboard.just_released(KEY_ESC) or Keyboard.just_released(KEY_BACKSPACE) then
        self.m_Stack:change_state(PlatformerTitleState:Create(self.m_Stack, self.m_Platformer))
    end
end
