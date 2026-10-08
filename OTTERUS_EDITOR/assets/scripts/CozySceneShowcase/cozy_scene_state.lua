-- Cozy Scene State for StateStack integration

CozySceneState = {}
CozySceneState.__index = CozySceneState

function CozySceneState:Create(stack, platformer)
    local this = {
        m_Stack      = stack,
        m_Platformer = platformer,
        m_Scene      = nil
    }

    local state = State("cozy_scene_state")
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

function CozySceneState:OnEnter()
    Camera.get().set_position(vec2(0, 0))
    Camera.get().set_scale(1)
    self.m_Scene = CozySceneShowcase:Create()
end

function CozySceneState:OnExit()
    if self.m_Scene then
        self.m_Scene:Destroy()
        self.m_Scene = nil
    end
end

function CozySceneState:OnUpdate(dt)
    if self.m_Scene then
        self.m_Scene:update(dt)
    end
end

function CozySceneState:OnRender()
end

function CozySceneState:HandleInputs()
    -- ESC or BACKSPACE returns to the main title screen
    if Keyboard.just_released(KEY_ESC) or Keyboard.just_released(KEY_BACKSPACE) then
        self.m_Stack:change_state(PlatformerTitleState:Create(self.m_Stack, self.m_Platformer))
    end
end
