AsteroidsTriggerSystem = Subscriber:Create()

function AsteroidsTriggerSystem:Create(game)
    local this = CreateObject(AsteroidsTriggerSystem):new({
        m_Game = game
    })
    return this
end

function AsteroidsTriggerSystem:OnCollision(obj_a, obj_b)
    if not obj_a or not obj_b then return end

    local isTrigA = obj_a.isTrigger or obj_a.bTrigger
    local isTrigB = obj_b.isTrigger or obj_b.bTrigger

    if isTrigA and obj_b.group == "asteroids" then
        self:HandleProjectileAsteroidHit(obj_a.entityId, obj_b.entityId)
    elseif isTrigB and obj_a.group == "asteroids" then
        self:HandleProjectileAsteroidHit(obj_b.entityId, obj_a.entityId)
    end
end

function AsteroidsTriggerSystem:HandleProjectileAsteroidHit(projId, astId)
    if self.m_Game then
        self.m_Game:OnProjectileHitAsteroid(projId, astId)
    end
end

ShipCollisionSubscriber = Subscriber:Create()

function ShipCollisionSubscriber:Create(game)
    local this = CreateObject(ShipCollisionSubscriber):new({
        m_Game = game
    })
    return this
end

function ShipCollisionSubscriber:OnCollision(obj_a, obj_b)
    if not obj_a or not obj_b then return end

    local isShipA = (obj_a.group == "player" or obj_a.tag == "ship")
    local isShipB = (obj_b.group == "player" or obj_b.tag == "ship")

    if isShipA and obj_b.group == "asteroids" then
        self:HandleShipAsteroidHit(obj_a.entityId, obj_b.entityId)
    elseif isShipB and obj_a.group == "asteroids" then
        self:HandleShipAsteroidHit(obj_b.entityId, obj_a.entityId)
    end
end

function ShipCollisionSubscriber:HandleShipAsteroidHit(shipId, astId)
    if self.m_Game then
        self.m_Game:OnShipHitAsteroid(shipId, astId)
    end
end

AsteroidsCollisionSystem = {}
AsteroidsCollisionSystem.__index = AsteroidsCollisionSystem

function AsteroidsCollisionSystem:Create(game)
    local this = {
        m_Game = game,
        m_CollisionEvent = CollisionEvent:Create()
    }
    setmetatable(this, self)

    this.m_TriggerSub = AsteroidsTriggerSystem:Create(game)
    this.m_ShipSub = ShipCollisionSubscriber:Create(game)

    this.m_CollisionEvent:SubscribeToEvent(this.m_TriggerSub)
    this.m_CollisionEvent:SubscribeToEvent(this.m_ShipSub)

    return this
end

function AsteroidsCollisionSystem:GetCircleInfo(entId)
    local ent = Entity(entId)
    local tf = ent:get_component(Transform)
    local sp = ent:get_component(Sprite)
    local cc = ent:get_component(CircleCollider)

    if not tf or not sp or not cc then
        return nil, 0
    end

    local radius = cc.radius * tf.scale.x
    local center = vec2(
        tf.position.x + (sp.width * tf.scale.x * 0.5),
        tf.position.y + (sp.height * tf.scale.y * 0.5)
    )
    return center, radius
end

function AsteroidsCollisionSystem:CheckCircleOverlap(centerA, radA, centerB, radB)
    local dx = centerA.x - centerB.x
    local dy = centerA.y - centerB.y
    local distSq = (dx * dx) + (dy * dy)
    local radSum = radA + radB
    return distSq <= (radSum * radSum)
end

function AsteroidsCollisionSystem:Update(ship, asteroids, projectiles)
    for pIdx = #projectiles, 1, -1 do
        local proj = projectiles[pIdx]
        if proj and not proj:IsDead() then
            local pCenter, pRad = self:GetCircleInfo(proj.m_EntityID)
            if pCenter then
                for aIdx = #asteroids, 1, -1 do
                    local ast = asteroids[aIdx]
                    if ast and not ast:IsDead() then
                        local aCenter, aRad = self:GetCircleInfo(ast.m_EntityID)
                        if aCenter and self:CheckCircleOverlap(pCenter, pRad, aCenter, aRad) then
                            self.m_CollisionEvent:EmitEvent(proj.m_ObjectData, ast.m_ObjectData)
                            if proj:IsDead() then
                                break
                            end
                        end
                    end
                end
            end
        end
    end

    if ship and not ship:IsDead() then
        local sCenter, sRad = self:GetCircleInfo(ship.m_EntityID)
        if sCenter then
            for aIdx = #asteroids, 1, -1 do
                local ast = asteroids[aIdx]
                if ast and not ast:IsDead() then
                    local aCenter, aRad = self:GetCircleInfo(ast.m_EntityID)
                    if aCenter and self:CheckCircleOverlap(sCenter, sRad, aCenter, aRad) then
                        self.m_CollisionEvent:EmitEvent(ship.m_ObjectData, ast.m_ObjectData)
                        if ship:IsDead() then
                            break
                        end
                    end
                end
            end
        end
    end
end
