-- Title & Scene Selection State

PlatformerTitleState = {}
PlatformerTitleState.__index = PlatformerTitleState

function PlatformerTitleState:Create(stack, platformer)
    local this = {
        m_Stack         = stack,
        m_Platformer    = platformer,
        m_TitleEntity   = nil,
        m_Option1Entity = nil,
        m_Option2Entity = nil
    }

    local state = State("platformer_title_state")
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

function PlatformerTitleState:OnEnter()
    -- Reset camera to origin for the title menu
    Camera.get().set_position(vec2(0, 0))
    Camera.get().set_scale(1)

    -- Hide game HUD while on title screen
    if self.m_Platformer then
        self.m_Platformer:set_ui_visible(false)
    end

    -- Title: "Otterus2D Game Hub" (X=170, Y=140)
    self.m_TitleEntity = Entity("Title_Text", "UI")
    self.m_TitleEntity:add_component(Transform(vec2(170, 140), vec2(1, 1), 0))
    self.m_TitleEntity:add_component(TextComponent("Otterus2D Game Hub", "pixel", 0, -1.0, Color(255, 255, 255, 255)))

    -- Option 1: "[1] Play Platformer" (X=170, Y=210)
    self.m_Option1Entity = Entity("Option1_Text", "UI")
    self.m_Option1Entity:add_component(Transform(vec2(170, 210), vec2(1, 1), 0))
    self.m_Option1Entity:add_component(TextComponent("[1 / Enter] Play Platformer", "pixel", 0, -1.0, Color(255, 200, 0, 255)))

    -- Option 2: Cozy Scene Showcase (X=170, Y=260)
    local hasCozyAssets = AssetLoader.HasTexture("ground_tiles") and (CozySceneState ~= nil)
    local option2Text = hasCozyAssets and "[2] Cozy Scene Showcase" or "[2] Cozy Scene (Unavailable)"
    local option2Color = hasCozyAssets and Color(255, 255, 255, 255) or Color(120, 120, 120, 255)

    self.m_Option2Entity = Entity("Option2_Text", "UI")
    self.m_Option2Entity:add_component(Transform(vec2(170, 260), vec2(1, 1), 0))
    self.m_Option2Entity:add_component(TextComponent(option2Text, "pixel", 0, -1.0, option2Color))

    -- Option 3: Asteroids (X=170, Y=310)
    self.m_Option3Entity = Entity("Option3_Text", "UI")
    self.m_Option3Entity:add_component(Transform(vec2(170, 310), vec2(1, 1), 0))
    self.m_Option3Entity:add_component(TextComponent("[3] Asteroids Arcade", "pixel", 0, -1.0, Color(100, 200, 255, 255)))
end

function PlatformerTitleState:OnExit()
    if self.m_TitleEntity then
        self.m_TitleEntity:kill()
        self.m_TitleEntity = nil
    end

    if self.m_Option1Entity then
        self.m_Option1Entity:kill()
        self.m_Option1Entity = nil
    end

    if self.m_Option2Entity then
        self.m_Option2Entity:kill()
        self.m_Option2Entity = nil
    end

    if self.m_Option3Entity then
        self.m_Option3Entity:kill()
        self.m_Option3Entity = nil
    end
end

function PlatformerTitleState:OnUpdate(dt)
end

function PlatformerTitleState:OnRender()
end

function PlatformerTitleState:HandleInputs()
    if Keyboard.just_released(KEY_ENTER) or Keyboard.just_pressed(KEY_ENTER)
    or Keyboard.just_released(KEY_1) or Keyboard.just_pressed(KEY_1)
    or Keyboard.just_released(KEY_SPACE) or Keyboard.just_pressed(KEY_SPACE) then
        if not self.m_Platformer then
            self.m_Platformer = PlatformerController:Create()
        end
        self.m_Stack:change_state(PlatformerGameState:Create(self.m_Stack, self.m_Platformer))
    elseif Keyboard.just_released(KEY_2) or Keyboard.just_pressed(KEY_2) then
        if AssetLoader.HasTexture("ground_tiles") and CozySceneState then
            if self.m_Platformer then
                self.m_Platformer:destroy()
                self.m_Platformer = nil
            end
            self.m_Stack:change_state(CozySceneState:Create(self.m_Stack, nil))
        else
            OT_warn("Cozy Scene assets are not installed.")
        end
    elseif Keyboard.just_released(KEY_3) or Keyboard.just_pressed(KEY_3) then
        if AsteroidsTitleState then
            if self.m_Platformer then
                self.m_Platformer:destroy()
                self.m_Platformer = nil
            end
            self.m_Stack:change_state(AsteroidsTitleState:Create(self.m_Stack))
        elseif AsteroidsGameState then
            if self.m_Platformer then
                self.m_Platformer:destroy()
                self.m_Platformer = nil
            end
            self.m_Stack:change_state(AsteroidsGameState:Create(self.m_Stack))
        end
    end
end
