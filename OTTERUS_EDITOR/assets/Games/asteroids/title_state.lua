AsteroidsTitleState = {}
AsteroidsTitleState.__index = AsteroidsTitleState

function AsteroidsTitleState:Create(stack)
    local this = {
        m_Stack = stack,
        m_Entities = {}
    }

    local state = State("asteroids_title_state")
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

function AsteroidsTitleState:OnEnter()
    Camera.get().set_position(vec2(0, 0))
    Camera.get().set_scale(1)

    LoadBackground()

    local titleEnt = Entity("Title", "UI")
    titleEnt:add_component(Transform(vec2(190, 140), vec2(1, 1), 0))
    titleEnt:add_component(TextComponent("ASTEROIDS 2D", "pixel", 0, -1.0, Color(255, 230, 80, 255)))
    table.insert(self.m_Entities, titleEnt)

    local startEnt = Entity("Start_Prompt", "UI")
    startEnt:add_component(Transform(vec2(180, 210), vec2(1, 1), 0))
    startEnt:add_component(TextComponent("[Enter / Space] Start Game", "pixel", 0, -1.0, Color(100, 255, 120, 255)))
    table.insert(self.m_Entities, startEnt)

    local hsText = "High Score: " .. tostring(gData:GetHighScore())
    local hsEnt = Entity("High_Score", "UI")
    hsEnt:add_component(Transform(vec2(220, 260), vec2(1, 1), 0))
    hsEnt:add_component(TextComponent(hsText, "pixel", 0, -1.0, Color(255, 255, 255, 255)))
    table.insert(self.m_Entities, hsEnt)

    local ctrlEnt = Entity("Controls", "UI")
    ctrlEnt:add_component(Transform(vec2(90, 310), vec2(1, 1), 0))
    ctrlEnt:add_component(TextComponent("W: Thrust | A/D: Turn | Space: Fire | H: Jump", "pixel", 0, -1.0, Color(180, 200, 230, 255)))
    table.insert(self.m_Entities, ctrlEnt)

    local backEnt = Entity("Back", "UI")
    backEnt:add_component(Transform(vec2(200, 360), vec2(1, 1), 0))
    backEnt:add_component(TextComponent("[Esc] Return to Hub", "pixel", 0, -1.0, Color(150, 150, 150, 255)))
    table.insert(self.m_Entities, backEnt)
end

function AsteroidsTitleState:OnExit()
    for _, ent in ipairs(self.m_Entities) do
        ent:kill()
    end
    self.m_Entities = {}
    ClearBackground()
end

function AsteroidsTitleState:OnUpdate(dt)
end

function AsteroidsTitleState:HandleInputs()
    if Keyboard.just_released(KEY_ENTER) or Keyboard.just_pressed(KEY_ENTER)
    or Keyboard.just_released(KEY_SPACE) or Keyboard.just_pressed(KEY_SPACE) then
        self.m_Stack:change_state(AsteroidsGameState:Create(self.m_Stack))
    elseif Keyboard.just_released(KEY_ESC) or Keyboard.just_released(KEY_BACKSPACE) then
        if PlatformerTitleState then
            self.m_Stack:change_state(PlatformerTitleState:Create(self.m_Stack, nil))
        else
            self.m_Stack:pop()
        end
    end
end
