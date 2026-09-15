AnimalWanderer = {}
AnimalWanderer.__index = AnimalWanderer

-- Animation row constants corresponding to spritesheet rows
local ANIM_DOWN       = 0
local ANIM_UP         = 1
local ANIM_LEFT       = 2
local ANIM_RIGHT      = 3
local ANIM_IDLE_DOWN  = 4

function AnimalWanderer:Create(params)
    params = params or {}
    local entity_ref = params.entity_id or params.entity
    local entity = (type(entity_ref) == "number") and Entity(entity_ref) or entity_ref

    local spawn_x = 200
    local spawn_y = 200
    if entity then
        local transform = entity:get_component(Transform)
        if transform then
            spawn_x = transform.position.x
            spawn_y = transform.position.y
        end
    end

    local radius = params.radius or 80

    local this = {
        m_Entity = entity,
        m_EntityId = (type(entity_ref) == "number") and entity_ref or nil,
        m_Speed = params.speed or 40.0, -- pixels per second
        -- Bounding limits (can use radius around spawn, or explicit min/max)
        m_MinX = params.min_x or (spawn_x - radius),
        m_MaxX = params.max_x or (spawn_x + radius),
        m_MinY = params.min_y or (spawn_y - radius),
        m_MaxY = params.max_y or (spawn_y + radius),
        -- Wait time at destination (in seconds)
        m_WaitMin = params.wait_min or 0.5,
        m_WaitMax = params.wait_max or 2.0,
        m_WaitTimer = 0.0,
        m_IsWaiting = false,
        m_ArriveDist = params.arrive_dist or 3.0,
        m_CurrentRow = -1,
        m_Target = nil
    }

    setmetatable(this, self)

    -- Set initial idle state and pick first target
    this:SetAnimationRow(ANIM_IDLE_DOWN)
    this:PickNewTarget()

    return this
end

function AnimalWanderer:SetAnimationRow(row)
    if self.m_CurrentRow == row then return end
    self.m_CurrentRow = row

    local entity = self.m_Entity
    if not entity and self.m_EntityId then
        entity = Entity(self.m_EntityId)
        self.m_Entity = entity
    end
    if not entity then return end

    local sprite = entity:get_component(Sprite)
    if sprite then
        sprite.start_y = row
        sprite:generate_uvs()
    end
end

function AnimalWanderer:PickNewTarget()
    local min_x = math.min(self.m_MinX, self.m_MaxX)
    local max_x = math.max(self.m_MinX, self.m_MaxX)
    local min_y = math.min(self.m_MinY, self.m_MaxY)
    local max_y = math.max(self.m_MinY, self.m_MaxY)

    local tx = math.random(math.floor(min_x), math.floor(max_x))
    local ty = math.random(math.floor(min_y), math.floor(max_y))

    self.m_Target = vec2(tx, ty)
    self.m_IsWaiting = false
end

function AnimalWanderer:Update(dt)
    local entity = self.m_Entity
    if not entity and self.m_EntityId then
        entity = Entity(self.m_EntityId)
        self.m_Entity = entity
    end
    if not entity then return end

    local transform = entity:get_component(Transform)
    if not transform then return end

    -- If resting/idle at destination
    if self.m_IsWaiting then
        self:SetAnimationRow(ANIM_IDLE_DOWN)
        self.m_WaitTimer = self.m_WaitTimer - dt
        if self.m_WaitTimer <= 0 then
            self:PickNewTarget()
        end
        return
    end

    -- Calculate distance vector to target
    local dx = self.m_Target.x - transform.position.x
    local dy = self.m_Target.y - transform.position.y
    local dist = math.sqrt((dx * dx) + (dy * dy))

    if dist <= self.m_ArriveDist then
        -- Target location reached!
        transform.position.x = self.m_Target.x
        transform.position.y = self.m_Target.y

        -- Pause and switch to idle
        self.m_IsWaiting = true
        self:SetAnimationRow(ANIM_IDLE_DOWN)

        if self.m_WaitMax > 0 then
            self.m_WaitTimer = self.m_WaitMin + (math.random() * (self.m_WaitMax - self.m_WaitMin))
        else
            self:PickNewTarget()
        end
    else
        -- Determine 4-way direction from movement vector
        if math.abs(dx) >= math.abs(dy) then
            if dx > 0 then
                self:SetAnimationRow(ANIM_RIGHT) -- 3 -> Right
            else
                self:SetAnimationRow(ANIM_LEFT)  -- 2 -> Left
            end
        else
            if dy > 0 then
                self:SetAnimationRow(ANIM_DOWN)  -- 0 -> Down
            else
                self:SetAnimationRow(ANIM_UP)    -- 1 -> Up
            end
        end

        -- Move toward target
        local step = self.m_Speed * dt
        if step > dist then
            step = dist
        end

        local nx = dx / dist
        local ny = dy / dist

        transform.position.x = transform.position.x + (nx * step)
        transform.position.y = transform.position.y + (ny * step)
    end
end

---------------------------------------------------------------------
-- Manager to easily handle multiple animals together
---------------------------------------------------------------------
AnimalWanderManager = {}
AnimalWanderManager.__index = AnimalWanderManager

function AnimalWanderManager:Create()
    local this = {
        m_Animals = {}
    }
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
