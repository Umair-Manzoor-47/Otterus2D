-- Animal wander controller and collision avoidance

local METERS_TO_PIXELS = 12.0
local PIXELS_TO_METERS = 1.0 / METERS_TO_PIXELS

-- Direction rows: 0=Down, 1=Up, 2=Left, 3=Right, 4=Idle
local DIR_DOWN  = 0
local DIR_UP    = 1
local DIR_LEFT  = 2
local DIR_RIGHT = 3
local DIR_IDLE  = 4

local STATE_IDLE   = 0
local STATE_MOVING = 1

AnimalWanderer = {}
AnimalWanderer.__index = AnimalWanderer

function AnimalWanderer:Create(params)
    params = params or {}
    local entity = (type(params.entity) == "number") and Entity(params.entity) or params.entity

    local spawn_x, spawn_y = 200, 200
    local bodyRadius = params.body_radius or params.avoid_radius
    if entity then
        local transform = entity:get_component(Transform)
        if transform then
            spawn_x = transform.position.x
            spawn_y = transform.position.y
        end
        if not bodyRadius then
            local collider = entity:get_component(BoxCollider)
            if collider then
                bodyRadius = math.max(collider.width, collider.height) * 0.5
            end
        end
    end

    local radius = params.radius or 80
    local raw_min_x = params.min_x or (spawn_x - radius)
    local raw_max_x = params.max_x or (spawn_x + radius)
    local raw_min_y = params.min_y or (spawn_y - radius)
    local raw_max_y = params.max_y or (spawn_y + radius)

    local this = {
        m_Entity         = entity,
        m_Physics        = nil,
        m_Manager        = params.manager,
        m_Speed          = params.speed or 25.0,
        m_BodyRadius     = bodyRadius or 10.0,
        m_MinX           = math.floor(math.min(raw_min_x, raw_max_x)),
        m_MaxX           = math.floor(math.max(raw_min_x, raw_max_x)),
        m_MinY           = math.floor(math.min(raw_min_y, raw_max_y)),
        m_MaxY           = math.floor(math.max(raw_min_y, raw_max_y)),
        m_WaitMin        = params.wait_min or 1.5,
        m_WaitMax        = params.wait_max or 3.5,
        m_WaitTimer      = 0.0,
        m_State          = STATE_IDLE,
        m_CurrentDir     = DIR_IDLE,
        m_CurrentRow     = -1,
        m_CurrentAxis    = "X",
        m_Target         = vec2(spawn_x, spawn_y),
        m_ArriveDist     = params.arrive_dist or 4.0,
        m_StuckTimer     = 0.0,
        m_MoveTimeout    = 0.0,
        m_LastPos        = nil,
        m_WalkFrameRate  = params.walk_fps or 5,
        m_IdleFrameRate  = params.idle_fps or 3
    }

    setmetatable(this, self)

    -- Stagger initial wait so animals do not all decide simultaneously
    this:EnterIdle(0.4 + math.random() * 1.5)
    return this
end

function AnimalWanderer:GetPhysics()
    if not self.m_Physics and self.m_Entity then
        self.m_Physics = self.m_Entity:get_component(PhysicsComponent)
    end
    return self.m_Physics
end

function AnimalWanderer:GetPosition()
    if not self.m_Entity then return nil end
    local transform = self.m_Entity:get_component(Transform)
    if transform then
        return transform.position
    end
    return nil
end

function AnimalWanderer:SetAnimationRow(row, frameRate)
    if not self.m_Entity then return end
    local sprite = self.m_Entity:get_component(Sprite)
    if sprite and (self.m_CurrentRow ~= row or sprite.start_y ~= row) then
        self.m_CurrentRow = row
        sprite.start_y = row
        sprite:generate_uvs()

        local anim = self.m_Entity:get_component(Animation)
        if anim then
            anim:reset()
        end
    end

    if frameRate then
        local anim = self.m_Entity:get_component(Animation)
        if anim and anim.frame_rate ~= frameRate then
            anim.frame_rate = frameRate
        end
    end
end

function AnimalWanderer:EnterIdle(duration)
    self.m_State       = STATE_IDLE
    self.m_CurrentDir  = DIR_IDLE
    self.m_WaitTimer   = duration or (self.m_WaitMin + math.random() * (self.m_WaitMax - self.m_WaitMin))
    self.m_StuckTimer  = 0.0
    self.m_MoveTimeout = 0.0

    local physics = self:GetPhysics()
    if physics then
        physics:set_linear_velocity(vec2(0, 0))
    end

    -- Calm resting/grazing animation (Row 4): no switching or twitching while deciding
    self:SetAnimationRow(DIR_IDLE, self.m_IdleFrameRate)
end

function AnimalWanderer:PickNewTarget()
    local curPos = self:GetPosition()
    if not curPos then return end

    local bestTx = curPos.x
    local bestTy = curPos.y
    local found = false

    -- Check if crowded by any neighboring animals
    local avoidDx, avoidDy = 0, 0
    if self.m_Manager then
        for _, other in ipairs(self.m_Manager.m_Animals) do
            if other ~= self then
                local op = other:GetPosition()
                if op then
                    local ddx = curPos.x - op.x
                    local ddy = curPos.y - op.y
                    local d = math.sqrt(ddx * ddx + ddy * ddy)
                    local danger = (self.m_BodyRadius or 10) + (other.m_BodyRadius or 10) + 12.0
                    if d < danger and d > 0.001 then
                        avoidDx = avoidDx + (ddx / d)
                        avoidDy = avoidDy + (ddy / d)
                    end
                end
            end
        end
    end

    -- If crowded, bias next target away from neighbors
    if math.abs(avoidDx) > 0.01 or math.abs(avoidDy) > 0.01 then
        local step = 32.0
        local tx = math.floor(curPos.x + avoidDx * step)
        local ty = math.floor(curPos.y + avoidDy * step)
        tx = math.max(self.m_MinX, math.min(self.m_MaxX, tx))
        ty = math.max(self.m_MinY, math.min(self.m_MaxY, ty))
        bestTx = tx
        bestTy = ty
        found = true
    else
        -- Pick random target at least minStep away so we don't take 1-pixel micro steps
        local minStep = 24.0
        for _ = 1, 10 do
            local tx = math.random(self.m_MinX, self.m_MaxX)
            local ty = math.random(self.m_MinY, self.m_MaxY)
            local dx = tx - curPos.x
            local dy = ty - curPos.y
            if math.sqrt(dx * dx + dy * dy) >= minStep then
                bestTx = tx
                bestTy = ty
                found = true
                break
            end
        end
    end

    if not found then
        bestTx = math.random(self.m_MinX, self.m_MaxX)
        bestTy = math.random(self.m_MinY, self.m_MaxY)
    end

    self.m_Target = vec2(bestTx, bestTy)
    local dx = self.m_Target.x - curPos.x
    local dy = self.m_Target.y - curPos.y

    if math.abs(dx) <= self.m_ArriveDist and math.abs(dy) <= self.m_ArriveDist then
        self:EnterIdle(1.0)
        return
    end

    -- Primary axis: choose axis with greatest remaining distance
    if math.abs(dx) >= math.abs(dy) then
        self.m_CurrentAxis = "X"
    else
        self.m_CurrentAxis = "Y"
    end

    self.m_State       = STATE_MOVING
    self.m_StuckTimer  = 0.0
    self.m_MoveTimeout = 0.0
    self.m_LastPos     = vec2(curPos.x, curPos.y)
end

function AnimalWanderer:Update(dt)
    if not self.m_Entity then return end
    local transform = self.m_Entity:get_component(Transform)
    if not transform then return end

    local physics = self:GetPhysics()

    if self.m_State == STATE_IDLE then
        if physics then
            physics:set_linear_velocity(vec2(0, 0))
        end
        self:SetAnimationRow(DIR_IDLE, self.m_IdleFrameRate)

        self.m_WaitTimer = self.m_WaitTimer - dt
        if self.m_WaitTimer <= 0 then
            self:PickNewTarget()
        end
        return
    end

    local curPos = transform.position
    local dx = self.m_Target.x - curPos.x
    local dy = self.m_Target.y - curPos.y

    if self.m_CurrentAxis == "X" then
        if math.abs(dx) <= self.m_ArriveDist then
            if math.abs(dy) > self.m_ArriveDist then
                self.m_CurrentAxis = "Y"
                self.m_StuckTimer  = 0.0
            else
                self:EnterIdle()
                return
            end
        end
    else
        if math.abs(dy) <= self.m_ArriveDist then
            if math.abs(dx) > self.m_ArriveDist then
                self.m_CurrentAxis = "X"
                self.m_StuckTimer  = 0.0
            else
                self:EnterIdle()
                return
            end
        end
    end

    local targetDir = DIR_IDLE
    local vx = 0.0
    local vy = 0.0

    if self.m_CurrentAxis == "X" then
        local curDx = self.m_Target.x - curPos.x
        if curDx > 0 then
            targetDir = DIR_RIGHT
            vx = self.m_Speed
        else
            targetDir = DIR_LEFT
            vx = -self.m_Speed
        end
    else
        local curDy = self.m_Target.y - curPos.y
        if curDy > 0 then
            targetDir = DIR_DOWN
            vy = self.m_Speed
        else
            targetDir = DIR_UP
            vy = -self.m_Speed
        end
    end

    -- Avoid other animals in path
    if self.m_Manager then
        local blocked, _ = self.m_Manager:IsPathBlocked(self, targetDir)
        if blocked then
            local altAxis = (self.m_CurrentAxis == "X") and "Y" or "X"
            local altDist = (altAxis == "X") and (self.m_Target.x - curPos.x) or (self.m_Target.y - curPos.y)

            if math.abs(altDist) > self.m_ArriveDist then
                local altDir = DIR_IDLE
                if altAxis == "X" then
                    altDir = (altDist > 0) and DIR_RIGHT or DIR_LEFT
                else
                    altDir = (altDist > 0) and DIR_DOWN or DIR_UP
                end

                local altBlocked = self.m_Manager:IsPathBlocked(self, altDir)
                if not altBlocked then
                    self.m_CurrentAxis = altAxis
                    targetDir = altDir
                    if altAxis == "X" then
                        vx = (targetDir == DIR_RIGHT) and self.m_Speed or -self.m_Speed
                        vy = 0.0
                    else
                        vx = 0.0
                        vy = (targetDir == DIR_DOWN) and self.m_Speed or -self.m_Speed
                    end
                else
                    self:EnterIdle(1.2 + math.random() * 1.5)
                    return
                end
            else
                self:EnterIdle(1.2 + math.random() * 1.5)
                return
            end
        end
    end

    self.m_CurrentDir = targetDir
    self:SetAnimationRow(targetDir, self.m_WalkFrameRate)

    if physics then
        local vx_meters = vx * PIXELS_TO_METERS
        local vy_meters = vy * PIXELS_TO_METERS
        physics:set_linear_velocity(vec2(vx_meters, vy_meters))
    else
        transform.position.x = transform.position.x + (vx * dt)
        transform.position.y = transform.position.y + (vy * dt)
    end

    self.m_MoveTimeout = self.m_MoveTimeout + dt
    if self.m_MoveTimeout > 6.0 then
        self:EnterIdle(1.0)
        return
    end

    if self.m_LastPos then
        local distMoved = math.sqrt((curPos.x - self.m_LastPos.x)^2 + (curPos.y - self.m_LastPos.y)^2)
        if distMoved < 0.25 then
            self.m_StuckTimer = self.m_StuckTimer + dt
            if self.m_StuckTimer > 0.8 then
                self.m_StuckTimer = 0.0
                self:EnterIdle(1.0 + math.random() * 1.0)
                return
            end
        else
            self.m_StuckTimer = 0.0
        end
    end
    self.m_LastPos = vec2(curPos.x, curPos.y)
end

AnimalWanderManager = {}
AnimalWanderManager.__index = AnimalWanderManager

function AnimalWanderManager:Create()
    local this = { m_Animals = {} }
    setmetatable(this, self)
    return this
end

function AnimalWanderManager:Add(entity_or_id, params)
    local p = {}
    if params then
        for k, v in pairs(params) do p[k] = v end
    end
    p.entity = entity_or_id
    p.manager = self

    -- Infer frame rates if not specified
    if not p.walk_fps then
        if entity_or_id and type(entity_or_id) ~= "number" then
            local anim = entity_or_id:get_component(Animation)
            if anim then p.walk_fps = anim.frame_rate end
        end
    end

    local wanderer = AnimalWanderer:Create(p)
    table.insert(self.m_Animals, wanderer)
    return wanderer
end

function AnimalWanderManager:IsPathBlocked(wanderer, dir)
    local myPos = wanderer:GetPosition()
    if not myPos then return false, nil end

    local myRadius = wanderer.m_BodyRadius or 10.0
    -- Corridor width: lateral slice where collision would occur
    local corridor = myRadius * 1.4

    for _, other in ipairs(self.m_Animals) do
        if other ~= wanderer then
            local otherPos = other:GetPosition()
            if otherPos then
                local otherRadius = other.m_BodyRadius or 10.0
                -- Lookahead threshold: detect well before physical colliders touch
                local lookAhead = myRadius + otherRadius + 14.0

                if dir == DIR_RIGHT then
                    local dx = otherPos.x - myPos.x
                    local dy = math.abs(otherPos.y - myPos.y)
                    if dx > 0 and dx < lookAhead and dy < corridor then
                        return true, other
                    end
                elseif dir == DIR_LEFT then
                    local dx = myPos.x - otherPos.x
                    local dy = math.abs(otherPos.y - myPos.y)
                    if dx > 0 and dx < lookAhead and dy < corridor then
                        return true, other
                    end
                elseif dir == DIR_DOWN then
                    local dy = otherPos.y - myPos.y
                    local dx = math.abs(otherPos.x - myPos.x)
                    if dy > 0 and dy < lookAhead and dx < corridor then
                        return true, other
                    end
                elseif dir == DIR_UP then
                    local dy = myPos.y - otherPos.y
                    local dx = math.abs(otherPos.x - myPos.x)
                    if dy > 0 and dy < lookAhead and dx < corridor then
                        return true, other
                    end
                end
            end
        end
    end

    return false, nil
end

function AnimalWanderManager:Update(dt)
    for _, wanderer in ipairs(self.m_Animals) do
        wanderer:Update(dt)
    end
end
