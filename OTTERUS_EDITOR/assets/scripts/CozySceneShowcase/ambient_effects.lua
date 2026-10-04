-- Ambient effects: thunder and leaves rustle

ThunderController = {}
ThunderController.__index = ThunderController

function ThunderController:Create()
    local entity = Entity("ambient_lightning", "weather")
    local transform = entity:add_component(Transform(vec2(0, 0), vec2(1, 1), 0))
    local sprite = entity:add_component(Sprite("white_flash", 640, 480, 0, 0, 8))
    sprite.color = Color(235, 245, 255, 0)
    sprite:generate_uvs()

    local this = {
        m_Entity           = entity,
        m_Sprite           = sprite,
        m_NextThunderTimer = 7.0,
        m_FlashTimer       = 0.0,
        m_IsFlashing       = false,
        m_SoundPlayed      = false
    }

    setmetatable(this, self)
    return this
end

function ThunderController:Trigger()
    self.m_IsFlashing   = true
    self.m_FlashTimer   = 0.0
    self.m_SoundPlayed  = false
    self.m_NextThunderTimer = math.random(22, 38)
end

function ThunderController:Update(dt)
    if not self.m_IsFlashing then
        self.m_NextThunderTimer = self.m_NextThunderTimer - dt
        if self.m_NextThunderTimer <= 0 then
            self:Trigger()
        end
        return
    end

    self.m_FlashTimer = self.m_FlashTimer + dt
    local t = self.m_FlashTimer
    local alpha = 0.0

    if t < 0.07 then
        alpha = (t / 0.07) * 52.0
    elseif t < 0.14 then
        alpha = 52.0 - ((t - 0.07) / 0.07) * 34.0
    elseif t < 0.24 then
        alpha = 18.0 + ((t - 0.14) / 0.10) * 16.0
    elseif t < 0.58 then
        alpha = 34.0 * (1.0 - (t - 0.24) / 0.34)
    else
        alpha = 0.0
        self.m_IsFlashing = false
    end

    if t >= 0.18 and not self.m_SoundPlayed then
        Sound.play("thunder")
        self.m_SoundPlayed = true
    end

    self.m_Sprite.color = Color(235, 245, 255, math.floor(math.max(0, math.min(255, alpha))))
end

function ThunderController:Destroy()
    if self.m_Entity then
        self.m_Entity:kill()
        self.m_Entity = nil
    end
end


LeafRustleController = {}
LeafRustleController.__index = LeafRustleController

local TREE_CANOPY_SPAWNS = {
    { x1 = 40,  x2 = 180, y1 = 20, y2 = 60 },
    { x1 = 220, x2 = 420, y1 = 16, y2 = 55 },
    { x1 = 440, x2 = 580, y1 = 20, y2 = 60 },
    { x1 = 30,  x2 = 90,  y1 = 80, y2 = 180 },
    { x1 = 30,  x2 = 100, y1 = 220, y2 = 340 },
    { x1 = 530, x2 = 600, y1 = 80, y2 = 200 },
    { x1 = 540, x2 = 600, y1 = 240, y2 = 360 },
    { x1 = 350, x2 = 390, y1 = 255, y2 = 285 },
}

function LeafRustleController:Create(numLeaves)
    numLeaves = numLeaves or 7
    local this = {
        m_Leaves           = {},
        m_NextRustleTimer  = 4.5,
        m_BreezeTimer      = 0.0,
        m_BreezeMultiplier = 1.0
    }
    setmetatable(this, self)

    for i = 1, numLeaves do
        local leaf = this:SpawnLeaf(true)
        table.insert(this.m_Leaves, leaf)
    end

    return this
end

function LeafRustleController:PickSpawnPosition(scatterAcrossScreen)
    if scatterAcrossScreen then
        return math.random(30, 600), math.random(20, 440)
    end
    local zone = TREE_CANOPY_SPAWNS[math.random(1, #TREE_CANOPY_SPAWNS)]
    return math.random(zone.x1, zone.x2), math.random(zone.y1, zone.y2)
end

function LeafRustleController:SpawnLeaf(scatterAcrossScreen)
    local sx, sy = self:PickSpawnPosition(scatterAcrossScreen)

    local entity    = Entity("", "leaf")
    local transform = entity:add_component(Transform(vec2(sx, sy), vec2(1, 1), 0))
    local sprite    = entity:add_component(Sprite("leaf_particles", 16, 16, 0, 0, 7))
    sprite.color    = Color(255, 255, 255, math.random(185, 230))
    sprite:generate_uvs()

    entity:add_component(Animation(4, math.random(3, 5), 0, false, true))

    return {
        entity     = entity,
        transform  = transform,
        sprite     = sprite,
        baseX      = sx,
        y          = sy,
        vx         = math.random(22, 44),
        vy         = math.random(16, 30),
        swayAmp    = math.random(8, 14),
        swaySpeed  = math.random(18, 32) / 10.0,
        swayPhase  = math.random() * 6.28
    }
end

function LeafRustleController:ResetLeaf(leaf)
    local sx, sy = self:PickSpawnPosition(false)
    leaf.baseX     = sx
    leaf.y         = sy
    leaf.vx        = math.random(22, 44)
    leaf.vy        = math.random(16, 30)
    leaf.swayAmp   = math.random(8, 14)
    leaf.swaySpeed = math.random(18, 32) / 10.0
    leaf.swayPhase = math.random() * 6.28
    leaf.transform.position = vec2(sx, sy)
end

function LeafRustleController:TriggerRustle()
    Sound.play("leaves_rustle")
    self.m_BreezeTimer      = 3.8
    self.m_NextRustleTimer  = math.random(14, 24)
end

function LeafRustleController:Update(dt)
    self.m_NextRustleTimer = self.m_NextRustleTimer - dt
    if self.m_NextRustleTimer <= 0 then
        self:TriggerRustle()
    end

    if self.m_BreezeTimer > 0 then
        self.m_BreezeTimer = self.m_BreezeTimer - dt
        self.m_BreezeMultiplier = 1.0 + 0.45 * math.sin((self.m_BreezeTimer / 3.8) * 3.1415)
    else
        self.m_BreezeMultiplier = 1.0
    end

    local mult = self.m_BreezeMultiplier

    for _, leaf in ipairs(self.m_Leaves) do
        leaf.swayPhase = leaf.swayPhase + dt * leaf.swaySpeed * mult
        leaf.baseX     = leaf.baseX + leaf.vx * dt * mult
        leaf.y         = leaf.y + leaf.vy * dt * mult

        local currentX = leaf.baseX + math.sin(leaf.swayPhase) * leaf.swayAmp
        leaf.transform.position = vec2(currentX, leaf.y)

        if currentX > 645 or leaf.y > 485 then
            self:ResetLeaf(leaf)
        end
    end
end

function LeafRustleController:Destroy()
    for _, leaf in ipairs(self.m_Leaves) do
        if leaf.entity then
            leaf.entity:kill()
            leaf.entity = nil
        end
    end
    self.m_Leaves = {}
end


AmbientEffectsManager = {}
AmbientEffectsManager.__index = AmbientEffectsManager

function AmbientEffectsManager:Create()
    local this = {
        thunder = ThunderController:Create(),
        leaves  = LeafRustleController:Create(7)
    }
    setmetatable(this, self)
    return this
end

function AmbientEffectsManager:Update(dt)
    if self.thunder then self.thunder:Update(dt) end
    if self.leaves  then self.leaves:Update(dt) end
end

function AmbientEffectsManager:Destroy()
    if self.thunder then self.thunder:Destroy() end
    if self.leaves  then self.leaves:Destroy() end
end
