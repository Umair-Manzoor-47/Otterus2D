-- =============================================================================
--  Animal Wanderer & Manager
--  Handles wandering behavior and 4-way animations for ambient animals
--  Applied Principles: KISS, DRY, YAGNI
-- =============================================================================

local ANIM_DOWN      = 0
local ANIM_UP        = 1
local ANIM_LEFT      = 2
local ANIM_RIGHT     = 3
local ANIM_IDLE_DOWN = 4

-- ---------------------------------------------------------------------------
-- AnimalWanderer
-- ---------------------------------------------------------------------------
AnimalWanderer = {}
AnimalWanderer.__index = AnimalWanderer

function AnimalWanderer:Create(params)
    params = params or {}
    local entity = (type(params.entity) == "number") and Entity(params.entity) or params.entity

    local spawn_x, spawn_y = 200, 200
    if entity then
        local transform = entity:get_component(Transform)
        if transform then
            spawn_x = transform.position.x
            spawn_y = transform.position.y
        end
    end

    local radius = params.radius or 80
    local raw_min_x = params.min_x or (spawn_x - radius)
    local raw_max_x = params.max_x or (spawn_x + radius)
    local raw_min_y = params.min_y or (spawn_y - radius)
    local raw_max_y = params.max_y or (spawn_y + radius)

    local this = {
        m_Entity      = entity,
        m_Speed       = params.speed or 40.0,
        m_MinX        = math.floor(math.min(raw_min_x, raw_max_x)),
        m_MaxX        = math.floor(math.max(raw_min_x, raw_max_x)),
        m_MinY        = math.floor(math.min(raw_min_y, raw_max_y)),
        m_MaxY        = math.floor(math.max(raw_min_y, raw_max_y)),
        m_WaitMin     = params.wait_min or 0.5,
        m_WaitMax     = params.wait_max or 2.0,
        m_WaitTimer   = 0.0,
        m_IsWaiting   = false,
        m_ArriveDist  = params.arrive_dist or 3.0,
        m_CurrentRow  = -1,
        m_Target      = nil
    }

    setmetatable(this, self)
    this:SetAnimationRow(ANIM_IDLE_DOWN)
    this:PickNewTarget()
    return this
end

function AnimalWanderer:SetAnimationRow(row)
    if self.m_CurrentRow == row or not self.m_Entity then return end
    self.m_CurrentRow = row

    local sprite = self.m_Entity:get_component(Sprite)
    if sprite then
        sprite.start_y = row
        sprite:generate_uvs()
    end
end

function AnimalWanderer:PickNewTarget()
    local tx = math.random(self.m_MinX, self.m_MaxX)
    local ty = math.random(self.m_MinY, self.m_MaxY)
    self.m_Target    = vec2(tx, ty)
    self.m_IsWaiting = false
end

function AnimalWanderer:Update(dt)
    if not self.m_Entity then return end
    local transform = self.m_Entity:get_component(Transform)
    if not transform then return end

    -- Resting at destination
    if self.m_IsWaiting then
        self:SetAnimationRow(ANIM_IDLE_DOWN)
        self.m_WaitTimer = self.m_WaitTimer - dt
        if self.m_WaitTimer <= 0 then
            self:PickNewTarget()
        end
        return
    end

    -- Movement toward target
    local dx = self.m_Target.x - transform.position.x
    local dy = self.m_Target.y - transform.position.y
    local dist = math.sqrt((dx * dx) + (dy * dy))

    if dist <= self.m_ArriveDist then
        transform.position.x = self.m_Target.x
        transform.position.y = self.m_Target.y
        self.m_IsWaiting = true
        self:SetAnimationRow(ANIM_IDLE_DOWN)
        self.m_WaitTimer = self.m_WaitMin + (math.random() * (self.m_WaitMax - self.m_WaitMin))
    else
        -- Direction facing
        if math.abs(dx) >= math.abs(dy) then
            self:SetAnimationRow(dx > 0 and ANIM_RIGHT or ANIM_LEFT)
        else
            self:SetAnimationRow(dy > 0 and ANIM_DOWN or ANIM_UP)
        end

        local step = math.min(self.m_Speed * dt, dist)
        transform.position.x = transform.position.x + (dx / dist * step)
        transform.position.y = transform.position.y + (dy / dist * step)
    end
end

-- ---------------------------------------------------------------------------
-- AnimalWanderManager
-- ---------------------------------------------------------------------------
AnimalWanderManager = {}
AnimalWanderManager.__index = AnimalWanderManager

function AnimalWanderManager:Create()
    local this = { m_Animals = {} }
    setmetatable(this, self)
    return this
end

function AnimalWanderManager:Add(entity_or_id, params)
    params = params or {}
    params.entity = entity_or_id
    local wanderer = AnimalWanderer:Create(params)
    table.insert(self.m_Animals, wanderer)
    return wanderer
end

function AnimalWanderManager:Update(dt)
    for _, wanderer in ipairs(self.m_Animals) do
        wanderer:Update(dt)
    end
end
