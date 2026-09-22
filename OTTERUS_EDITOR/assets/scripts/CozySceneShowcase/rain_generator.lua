-- =============================================================================
--  Rain Generator
--  Particle system simulating ambient rain drops and splash animations
--  Applied Principles: KISS, DRY, YAGNI
-- =============================================================================

Rain = {}
Rain.__index = Rain

function Rain:Create(params)
    params = params or {}
    local initPos = params.position or vec2(0, 0)
    local vel     = params.velocity or vec2(200, 200)

    local entity    = Entity("", "rain")
    local transform = entity:add_component(Transform(initPos, vec2(1, 1), 0))
    local sprite    = entity:add_component(Sprite(params.texture or "rain", params.width or 16, params.height or 16, 0, 0, 6))
    sprite.color    = Color(255, 255, 255, params.alpha or 135)
    sprite:generate_uvs()

    local anim      = entity:add_component(Animation(0, 5, 1, false, false))
    local rigidBody = entity:add_component(RigidBody(vel))

    local timer = Timer()
    timer:start()

    local this = {
        m_Entity          = entity,
        m_Transform       = transform,
        m_Sprite          = sprite,
        m_Anim            = anim,
        m_RigidBody       = rigidBody,
        m_InitialPosition = initPos,
        m_MinVelocity     = params.min_velocity or 100,
        m_MaxVelocity     = params.max_velocity or 400,
        m_MinLifeTime     = params.min_life_time or 0,
        m_MaxLifeTime     = params.max_life_time or 1500,
        m_LifeTime        = params.life_time or 1000,
        m_NumFrames       = params.num_frames or 5,
        m_LifeTimer       = timer,
        m_Finishing       = false
    }

    setmetatable(this, self)
    return this
end

function Rain:LifeOver()
    return self.m_LifeTimer:elapsed_ms() > self.m_LifeTime
end

function Rain:Update(dt)
    if not self.m_Entity then return end
    self.m_Transform.position = self.m_Transform.position + (self.m_RigidBody.velocity * dt)
end

function Rain:Finish()
    if not self.m_Entity then return end

    if not self.m_Finishing then
        self.m_RigidBody.velocity = vec2(0, 0)
        self.m_Anim.num_frames    = self.m_NumFrames
        self.m_Finishing          = true
    else
        -- Splash animation complete: reset raindrop to top
        if self.m_Anim.current_frame == self.m_Anim.num_frames - 1 then
            self.m_Anim:reset()
            self.m_Anim.num_frames    = 0
            self.m_Sprite.uvs.u       = 0
            self.m_Transform.position = self.m_InitialPosition

            local v = math.random(math.floor(self.m_MinVelocity), math.floor(self.m_MaxVelocity))
            self.m_RigidBody.velocity = vec2(v, v)
            self.m_LifeTime           = math.random(math.floor(self.m_MinLifeTime), math.floor(self.m_MaxLifeTime))

            self.m_LifeTimer:stop()
            self.m_LifeTimer:start()
            self.m_Finishing = false
        end
    end
end

function Rain:Destroy()
    if self.m_Entity then
        self.m_Entity:kill()
        self.m_Entity = nil
    end
end

-- ---------------------------------------------------------------------------
-- RainGenerator
-- ---------------------------------------------------------------------------
RainGenerator = {}
RainGenerator.__index = RainGenerator

function RainGenerator:Create(params)
    params = params or {}
    local this = {
        m_xOffset     = params.x_offset or 32,
        m_yOffset     = params.y_offset or 32,
        m_RainVelMin  = params.rain_vel_min or 100,
        m_RainVelMax  = params.rain_vel_max or 200,
        m_RainLifeMin = params.rain_life_min or 500,
        m_RainLifeMax = params.rain_life_max or 1500,
        m_RainTable   = {}
    }

    setmetatable(this, self)
    this:ReGenerate()
    return this
end

function RainGenerator:Update(dt)
    for _, drop in ipairs(self.m_RainTable) do
        drop:Update(dt)
        if drop:LifeOver() then
            drop:Finish()
        end
    end
end

function RainGenerator:ReGenerate()
    if #self.m_RainTable > 0 then return end

    local camScale  = Camera.get():scale()
    local maxWidth  = WindowWidth() * camScale
    local maxHeight = WindowHeight() * camScale

    local rows = math.floor(maxHeight / self.m_yOffset)
    local cols = math.floor(maxWidth / self.m_xOffset)

    for i = 0, rows do
        for j = 0, cols do
            local start_x = clamp(self.m_xOffset * j, 0, WindowWidth() - 1)
            local start_y = clamp(self.m_yOffset * i, 0, WindowHeight() - 1)
            local v       = math.random(self.m_RainVelMin, self.m_RainVelMax)

            local drop = Rain:Create({
                position  = vec2(start_x, start_y),
                velocity  = vec2(v, v),
                life_time = math.random(self.m_RainLifeMin, self.m_RainLifeMax)
            })
            table.insert(self.m_RainTable, drop)
        end
    end
end

function RainGenerator:Destroy()
    for _, drop in ipairs(self.m_RainTable) do
        drop:Destroy()
    end
    self.m_RainTable = {}
end