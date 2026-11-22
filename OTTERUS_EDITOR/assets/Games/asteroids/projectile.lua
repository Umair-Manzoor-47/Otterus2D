AsteroidLaser = {}
AsteroidLaser.__index = AsteroidLaser

function AsteroidLaser:Create(params)
    local defName = params.def or "proj_1"
    local pDef = ProjectileDefs[defName]
    assert(pDef, "Laser def not found: " .. tostring(defName))

    local entId = LoadEntityDef(pDef)
    local ent = Entity(entId)
    local transform = ent:get_component(Transform)
    local sprite = ent:get_component(Sprite)

    transform.position = params.start_pos
    transform.rotation = params.rotation

    local this = {
        m_EntityID = entId,
        m_Dir = params.dir,
        m_Speed = pDef.speed or 15,
        m_Lifetime = pDef.life_time or 1400,
        m_LifeTimer = Timer(),
        m_Dead = false,
        m_Width = sprite.width,
        m_Height = sprite.height,
        m_ObjectData = ObjectData("laser", "projectiles", true, true, true, entId)
    }

    this.m_LifeTimer:start()
    setmetatable(this, self)
    return this
end

function AsteroidLaser:Update(dt)
    if self.m_Dead then return end

    local ent = Entity(self.m_EntityID)
    local transform = ent:get_component(Transform)
    if not transform then return end

    transform.position = transform.position + self.m_Dir * self.m_Speed

    if transform.position.x < -20 or transform.position.x > SCREEN_WIDTH + 20
    or transform.position.y < -20 or transform.position.y > SCREEN_HEIGHT + 20 then
        self:Destroy()
        return
    end

    if self.m_LifeTimer:elapsed_ms() >= self.m_Lifetime then
        self:Destroy()
    end
end

function AsteroidLaser:Destroy()
    if self.m_Dead then return end
    self.m_Dead = true
    local ent = Entity(self.m_EntityID)
    ent:kill()
end

function AsteroidLaser:IsDead()
    return self.m_Dead
end
