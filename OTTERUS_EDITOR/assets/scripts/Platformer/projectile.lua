-- Projectile Class
-- Pure Lua projectile system for the Otterus2D Platformer

Projectile = {}
Projectile.__index = Projectile

Projectile.ActiveProjectiles = {}

function Projectile:Create(params)
    params = params or {}
    local this = {
        m_X         = params.x or 0.0,
        m_Y         = params.y or 0.0,
        m_DirX      = params.dirX or 1,
        m_DirY      = params.dirY or 0,
        m_Speed     = params.speed or 520.0,
        m_Lifetime  = params.lifetime or 2.0,
        m_SpinSpeed = params.spinSpeed or 900.0,
        m_Scale     = params.scale or 0.55,
        m_Texture   = params.texture or "ball",
        m_Layer     = params.layer or 3,
        m_Rotation  = 0.0,
        m_Elapsed   = 0.0,
        m_IsAlive   = true
    }

    local ent = Entity("NinjaProjectile", "projectile")
    ent:add_component(Transform(vec2(this.m_X, this.m_Y), vec2(this.m_Scale, this.m_Scale), 0))
    local sprite = ent:add_component(Sprite(this.m_Texture, 32, 32, 0, 0, this.m_Layer))
    sprite:generate_uvs()

    this.m_Entity   = ent
    this.m_EntityID = ent:id()

    setmetatable(this, self)
    table.insert(Projectile.ActiveProjectiles, this)
    return this
end

function Projectile:update(dt)
    if not self.m_IsAlive then return false end

    self.m_Elapsed = self.m_Elapsed + dt
    if self.m_Elapsed >= self.m_Lifetime then
        self:destroy()
        return false
    end

    -- Update authoritative position and rotation in Lua
    self.m_X        = self.m_X + (self.m_DirX * self.m_Speed * dt)
    self.m_Y        = self.m_Y + (self.m_DirY * self.m_Speed * dt)
    self.m_Rotation = (self.m_Rotation + (self.m_SpinSpeed * dt * self.m_DirX)) % 360

    -- Safety boundary check (screen / map limits)
    if self.m_X < -300 or self.m_X > 3500 or self.m_Y < -300 or self.m_Y > 1200 then
        self:destroy()
        return false
    end

    -- Always query the live TransformComponent fresh from the entity to prevent
    -- stale references when Entt reallocates storage during rapid entity creation.
    if self.m_Entity then
        local ok, hasComp = pcall(function() return self.m_Entity:has_component(Transform) end)
        if ok and hasComp then
            local ok2, tf = pcall(function() return self.m_Entity:get_component(Transform) end)
            if ok2 and tf then
                tf.position.x = self.m_X
                tf.position.y = self.m_Y
                tf.rotation   = self.m_Rotation
            end
        else
            self:destroy()
            return false
        end
    end

    return true
end

function Projectile:destroy()
    if not self.m_IsAlive then return end
    self.m_IsAlive = false
    if self.m_Entity then
        pcall(function() self.m_Entity:kill() end)
        self.m_Entity = nil
    end
end

function Projectile.Spawn(params)
    return Projectile:Create(params)
end

function Projectile.UpdateAll(dt)
    local i = 1
    while i <= #Projectile.ActiveProjectiles do
        local p = Projectile.ActiveProjectiles[i]
        if not p or not p:update(dt) then
            table.remove(Projectile.ActiveProjectiles, i)
        else
            i = i + 1
        end
    end
end

function Projectile.ClearAll()
    for _, p in ipairs(Projectile.ActiveProjectiles) do
        p:destroy()
    end
    Projectile.ActiveProjectiles = {}
end

function Projectile.Count()
    return #Projectile.ActiveProjectiles
end
