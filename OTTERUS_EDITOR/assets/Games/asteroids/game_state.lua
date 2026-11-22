AsteroidsGameState = {}
AsteroidsGameState.__index = AsteroidsGameState

function AsteroidsGameState:Create(stack)
    local this = {
        m_Stack = stack,
        m_Ship = nil,
        m_Asteroids = {},
        m_Projectiles = {},
        m_Collision = nil,
        m_IsPaused = false,
        m_UIEntities = {},
        m_ScoreText = nil,
        m_LivesText = nil,
        m_WaveText = nil,
        m_PauseText = nil,
        m_WaveSpawnDelay = 0,
        m_FpsAccumulator = 0
    }

    local state = State("asteroids_game_state")
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

function AsteroidsGameState:OnEnter()
    Camera.get().set_position(vec2(0, 0))
    Camera.get().set_scale(1)

    gData:Reset()

    LoadBackground()
    self:CreateUI()

    self.m_Ship = Ship:Create()
    self.m_Collision = AsteroidsCollisionSystem:Create(self)
    self.m_Asteroids = {}
    self.m_Projectiles = {}
    self.m_IsPaused = false

    self:SpawnWave(gData:GetWave())

    if AssetLoader and AssetLoader.PlayMusic then
        AssetLoader.PlayMusic("asteroids_bgm")
    elseif Music and Music.play then
        Music.play("bgm")
    end
end

function AsteroidsGameState:CreateUI()
    local scoreEnt = Entity("HUD_Score", "UI")
    scoreEnt:add_component(Transform(vec2(24, 20), vec2(1, 1), 0))
    self.m_ScoreText = scoreEnt:add_component(TextComponent("SCORE: 00000", "pixel", 0, -1.0, Color(255, 255, 255, 255)))
    table.insert(self.m_UIEntities, scoreEnt)

    local waveEnt = Entity("HUD_Wave", "UI")
    waveEnt:add_component(Transform(vec2(280, 20), vec2(1, 1), 0))
    self.m_WaveText = waveEnt:add_component(TextComponent("WAVE: 1", "pixel", 0, -1.0, Color(180, 220, 255, 255)))
    table.insert(self.m_UIEntities, waveEnt)

    local livesEnt = Entity("HUD_Lives", "UI")
    livesEnt:add_component(Transform(vec2(520, 20), vec2(1, 1), 0))
    self.m_LivesText = livesEnt:add_component(TextComponent("LIVES: 3", "pixel", 0, -1.0, Color(255, 200, 80, 255)))
    table.insert(self.m_UIEntities, livesEnt)

    local pauseEnt = Entity("HUD_Pause", "UI")
    pauseEnt:add_component(Transform(vec2(200, 240), vec2(1, 1), 0))
    self.m_PauseText = pauseEnt:add_component(TextComponent("PAUSED - [P] Resume", "pixel", 0, -1.0, Color(255, 255, 0, 255)))
    self.m_PauseText.isHidden = true
    table.insert(self.m_UIEntities, pauseEnt)
end

function AsteroidsGameState:SpawnWave(waveNumber)
    local count = 3 + waveNumber
    local shipPos = self.m_Ship and self.m_Ship:GetPosition() or vec2(320, 240)
    for _ = 1, count do
        local pos = GetRandomPosition(shipPos.x, shipPos.y, 140)
        local ast = Asteroid:Create("asteroid_big", pos)
        table.insert(self.m_Asteroids, ast)
    end
end

function AsteroidsGameState:OnExit()
    if Music and Music.stop then
        Music.stop()
    end

    if self.m_Ship then
        self.m_Ship:Destroy()
        self.m_Ship = nil
    end

    for _, ast in ipairs(self.m_Asteroids) do
        ast:Destroy()
    end
    self.m_Asteroids = {}

    for _, proj in ipairs(self.m_Projectiles) do
        proj:Destroy()
    end
    self.m_Projectiles = {}

    for _, ent in ipairs(self.m_UIEntities) do
        ent:kill()
    end
    self.m_UIEntities = {}

    ClearBackground()
end

local TARGET_DT = 1.0 / 60.0

function AsteroidsGameState:OnUpdate(dt)
    if self.m_IsPaused then return end

    local frameDt = (GetDeltaTime and GetDeltaTime() > 0) and GetDeltaTime() or (dt or (1.0 / 60.0))
    if frameDt > 0.1 then frameDt = 0.1 end

    self.m_FpsAccumulator = (self.m_FpsAccumulator or 0) + frameDt
    if self.m_FpsAccumulator < TARGET_DT then
        return
    end

    if self.m_FpsAccumulator > TARGET_DT * 4 then
        self.m_FpsAccumulator = TARGET_DT
    end

    while self.m_FpsAccumulator >= TARGET_DT do
        self:Tick(TARGET_DT)
        self.m_FpsAccumulator = self.m_FpsAccumulator - TARGET_DT
    end
end

function AsteroidsGameState:Tick(dt)
    if self.m_Ship then
        self.m_Ship:Update(dt)
        local newProjs = self.m_Ship:PopProjectiles()
        for _, p in ipairs(newProjs) do
            table.insert(self.m_Projectiles, p)
        end
    end

    for i = #self.m_Projectiles, 1, -1 do
        local proj = self.m_Projectiles[i]
        proj:Update(dt)
        if proj:IsDead() then
            table.remove(self.m_Projectiles, i)
        end
    end

    for i = #self.m_Asteroids, 1, -1 do
        local ast = self.m_Asteroids[i]
        ast:Update(dt)
        if ast:IsDead() then
            table.remove(self.m_Asteroids, i)
        end
    end

    if self.m_Collision then
        self.m_Collision:Update(self.m_Ship, self.m_Asteroids, self.m_Projectiles)
    end

    if #self.m_Asteroids == 0 then
        self.m_WaveSpawnDelay = self.m_WaveSpawnDelay + dt
        if self.m_WaveSpawnDelay >= 1.0 then
            self.m_WaveSpawnDelay = 0
            gData:NextWave()
            self:SpawnWave(gData:GetWave())
        end
    else
        self.m_WaveSpawnDelay = 0
    end

    if self.m_ScoreText then
        self.m_ScoreText.textStr = string.format("SCORE: %05d", gData:GetScore())
    end
    if self.m_LivesText then
        self.m_LivesText.textStr = "LIVES: " .. tostring(gData:NumLives())
    end
    if self.m_WaveText then
        self.m_WaveText.textStr = "WAVE: " .. tostring(gData:GetWave())
    end

    if gData:IsGameOver() then
        self.m_Stack:change_state(AsteroidsGameOverState:Create(self.m_Stack))
    end
end

function AsteroidsGameState:HandleInputs()
    if Keyboard.just_pressed(KEY_P) then
        self.m_IsPaused = not self.m_IsPaused
        if self.m_PauseText then
            self.m_PauseText.isHidden = not self.m_IsPaused
        end
    end

    if Keyboard.just_released(KEY_ESC) or Keyboard.just_released(KEY_BACKSPACE) then
        self.m_Stack:change_state(AsteroidsTitleState:Create(self.m_Stack))
        return
    end

    if self.m_Ship then
        self.m_Ship:HandleInputs()
    end
end

function AsteroidsGameState:OnProjectileHitAsteroid(projId, astId)
    for _, proj in ipairs(self.m_Projectiles) do
        if proj.m_EntityID == projId and not proj:IsDead() then
            proj:Destroy()
            break
        end
    end

    for _, ast in ipairs(self.m_Asteroids) do
        if ast.m_EntityID == astId and not ast:IsDead() then
            gData:AddScore(ast.m_Score)
            local fragments = ast:Split()
            for _, frag in ipairs(fragments) do
                table.insert(self.m_Asteroids, frag)
            end
            ast:Destroy()
            break
        end
    end
end

function AsteroidsGameState:OnShipHitAsteroid(shipId, astId)
    if self.m_Ship and self.m_Ship.m_EntityID == shipId then
        self.m_Ship:TakeHit()
    end
end
