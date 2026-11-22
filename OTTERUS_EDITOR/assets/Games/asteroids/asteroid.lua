Asteroid = {}
Asteroid.__index = Asteroid

function Asteroid:Create(defName, spawnPos)
    local aDef = AsteroidDefs[defName]
    assert(aDef, "Asteroid def does not exist: " .. tostring(defName))

    local entId = LoadEntityDef(aDef)
    local ent = Entity(entId)
    local transform = ent:get_component(Transform)
    local sprite = ent:get_component(Sprite)

    if spawnPos then
        transform.position = spawnPos
    else
        transform.position = GetRandomPosition(320, 240, 140)
    end

    local rotDir = math.random(0, 1) == 0 and -1 or 1
    local this = {
        m_Def = defName,
        m_Type = aDef.type,
        m_EntityID = entId,
        m_RotationDir = rotDir,
        m_Velocity = GetRandomVelocity(aDef.min_speed, aDef.max_speed),
        m_RotationSpeed = 0.8 + math.random() * 1.6,
        m_Dead = false,
        m_Score = aDef.score or (aDef.type == "big" and 20 or 50),
        m_Width = sprite.width,
        m_Height = sprite.height,
        m_ScaleX = transform.scale.x,
        m_ScaleY = transform.scale.y,
        m_ObjectData = ObjectData(defName, "asteroids", true, false, false, entId)
    }

    setmetatable(this, self)
    return this
end

function Asteroid:Update(dt)
    if self.m_Dead then return end

    local ent = Entity(self.m_EntityID)
    local transform = ent:get_component(Transform)
    if not transform then return end

    transform.rotation = transform.rotation + (self.m_RotationSpeed * self.m_RotationDir)
    transform.position = transform.position + self.m_Velocity
    CheckPosition(transform.position, self.m_Width * self.m_ScaleX, self.m_Height * self.m_ScaleY)
end

function Asteroid:Split()
    if self.m_Type == "big" then
        local ent = Entity(self.m_EntityID)
        local transform = ent:get_component(Transform)
        local pos = transform and vec2(transform.position.x, transform.position.y) or vec2(320, 240)
        local pieces = {}
        for _ = 1, 2 do
            local small = Asteroid:Create("asteroid_small", vec2(pos.x, pos.y))
            table.insert(pieces, small)
        end
        return pieces
    end
    return {}
end

function Asteroid:Destroy()
    if self.m_Dead then return end
    self.m_Dead = true
    local ent = Entity(self.m_EntityID)
    ent:kill()
end

function Asteroid:IsDead()
    return self.m_Dead
end
