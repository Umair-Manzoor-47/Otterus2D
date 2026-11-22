Ship = {}
Ship.__index = Ship

function Ship:Create(def)
    local sDef = def or ShipDefs["red_ship"]
    local entId = LoadEntityDef(sDef)
    local ent = Entity(entId)
    local sprite = ent:get_component(Sprite)
    local transform = ent:get_component(Transform)

    local this = {
        m_EntityID = entId,
        m_AngSpeed = 4.5,
        m_Velocity = vec2(0, 0),
        m_Acceleration = 0.35,
        m_Drag = 0.988,
        m_MaxSpeed = 8.5,
        m_ShootCooldown = 0.0,
        m_ShootCooldownTime = 0.16,
        m_InvincibleTimer = 0.0,
        m_InvincibleDuration = 3.0,
        m_Width = sprite.width,
        m_Height = sprite.height,
        m_ScaleX = transform and transform.scale.x or 0.65,
        m_ScaleY = transform and transform.scale.y or 0.65,
        m_ObjectData = ObjectData("ship", "player", true, false, true, entId),
        m_StateMachine = StateMachine(),
        m_PendingProjectiles = {},
        m_Dead = false
    }

    setmetatable(this, self)
    this:InitStateMachine()
    return this
end

function Ship:InitStateMachine()
    local normalState = State("normal")
    normalState:set_on_enter(function()
        local ent = Entity(self.m_EntityID)
        local sprite = ent:get_component(Sprite)
        if sprite then
            sprite.color.a = 255
        end
    end)
    normalState:set_on_update(function(dt)
        self:UpdateMovement(dt)
        self:UpdateCombat(dt)
    end)

    local invincibleState = State("invincible")
    invincibleState:set_on_enter(function()
        self.m_InvincibleTimer = 0.0
    end)
    invincibleState:set_on_update(function(dt)
        self:UpdateMovement(dt)
        self:UpdateCombat(dt)

        self.m_InvincibleTimer = self.m_InvincibleTimer + dt

        local ent = Entity(self.m_EntityID)
        local sprite = ent:get_component(Sprite)
        if sprite then
            if (math.floor(self.m_InvincibleTimer * 8) % 2) == 0 then
                sprite.color.a = 90
            else
                sprite.color.a = 240
            end
        end

        if self.m_InvincibleTimer >= self.m_InvincibleDuration then
            self.m_StateMachine:change_state("normal")
        end
    end)
    invincibleState:set_on_exit(function()
        local ent = Entity(self.m_EntityID)
        local sprite = ent:get_component(Sprite)
        if sprite then
            sprite.color.a = 255
        end
    end)

    local deadState = State("dead")
    deadState:set_on_enter(function()
        self.m_Velocity = vec2(0, 0)
        local ent = Entity(self.m_EntityID)
        local sprite = ent:get_component(Sprite)
        if sprite then
            sprite.color.a = 0
        end
    end)

    self.m_StateMachine:add_state(normalState)
    self.m_StateMachine:add_state(invincibleState)
    self.m_StateMachine:add_state(deadState)
    self.m_StateMachine:change_state("invincible")
end

function Ship:Update(dt)
    if self.m_Dead then return end
    dt = dt or 0.016
    self.m_StateMachine:update(dt)
end

function Ship:UpdateMovement(dt)
    local ent = Entity(self.m_EntityID)
    local transform = ent:get_component(Transform)
    if not transform then return end

    if Keyboard.pressed(KEY_A) or Keyboard.pressed(KEY_LEFT) then
        transform.rotation = transform.rotation - self.m_AngSpeed
    elseif Keyboard.pressed(KEY_D) or Keyboard.pressed(KEY_RIGHT) then
        transform.rotation = transform.rotation + self.m_AngSpeed
    end

    local radAngle = math.rad(transform.rotation)
    local fwd = vec2(math.cos(radAngle), math.sin(radAngle))

    if Keyboard.pressed(KEY_W) or Keyboard.pressed(KEY_UP) then
        self.m_Velocity = self.m_Velocity + fwd * self.m_Acceleration
        local speed = self.m_Velocity:length()
        if speed > self.m_MaxSpeed then
            self.m_Velocity = (self.m_Velocity / speed) * self.m_MaxSpeed
        end
    end

    if Keyboard.just_pressed(KEY_H) or Keyboard.just_pressed(KEY_LSHIFT) or Keyboard.just_pressed(KEY_RSHIFT) or self.m_TriggerHyperspace then
        self.m_TriggerHyperspace = false
        transform.position = GetRandomPosition(transform.position.x, transform.position.y, 90)
        self.m_Velocity = vec2(0, 0)
    end

    self.m_Velocity = self.m_Velocity * self.m_Drag
    transform.position = transform.position + self.m_Velocity
    CheckPosition(transform.position, self.m_Width * self.m_ScaleX, self.m_Height * self.m_ScaleY)
end

function Ship:HandleInputs()
    if Keyboard.just_pressed(KEY_SPACE) or Keyboard.just_pressed(KEY_ENTER) then
        self.m_TriggerShoot = true
    end
    if Keyboard.just_pressed(KEY_H) or Keyboard.just_pressed(KEY_LSHIFT) or Keyboard.just_pressed(KEY_RSHIFT) then
        self.m_TriggerHyperspace = true
    end
end

function Ship:UpdateCombat(dt)
    dt = dt or 0.016
    if self.m_ShootCooldown > 0 then
        self.m_ShootCooldown = self.m_ShootCooldown - dt
    end

    local wantShoot = self.m_TriggerShoot
                   or Keyboard.just_pressed(KEY_SPACE) or Keyboard.pressed(KEY_SPACE)
                   or Keyboard.just_pressed(KEY_ENTER) or Keyboard.pressed(KEY_ENTER)
    self.m_TriggerShoot = false

    if wantShoot and self.m_ShootCooldown <= 0 then
        self:FireLaser()
        self.m_ShootCooldown = self.m_ShootCooldownTime
    end
end

function Ship:FireLaser()
    local ent = Entity(self.m_EntityID)
    local transform = ent:get_component(Transform)
    if not transform then return end

    local radAngle = math.rad(transform.rotation)
    local fwd = vec2(math.cos(radAngle), math.sin(radAngle))

    local cx = transform.position.x + (self.m_Width * self.m_ScaleX * 0.5)
    local cy = transform.position.y + (self.m_Height * self.m_ScaleY * 0.5)
    local noseDist = self.m_Width * self.m_ScaleX * 0.5

    local spawnX = cx + (fwd.x * (noseDist + 4)) - 18
    local spawnY = cy + (fwd.y * (noseDist + 4)) - 6

    local proj = AsteroidLaser:Create({
        def = "proj_1",
        dir = fwd,
        start_pos = vec2(spawnX, spawnY),
        rotation = transform.rotation
    })

    table.insert(self.m_PendingProjectiles, proj)

    if AssetLoader and AssetLoader.PlaySound then
        AssetLoader.PlaySound("laser")
    elseif Sound and Sound.play then
        Sound.play("laser")
    end
end

function Ship:TakeHit()
    if self.m_Dead then return false end
    if self.m_StateMachine:current_state() == "invincible" then
        return false
    end
    if self.m_StateMachine:current_state() == "dead" then
        return false
    end

    gData:RemoveLife()

    if gData:IsGameOver() then
        self.m_Dead = true
        self.m_StateMachine:change_state("dead")
        return true
    end

    self:Respawn()
    return true
end

function Ship:Respawn()
    local ent = Entity(self.m_EntityID)
    local transform = ent:get_component(Transform)
    if transform then
        transform.position = vec2(320 - (self.m_Width * self.m_ScaleX * 0.5), 240 - (self.m_Height * self.m_ScaleY * 0.5))
        transform.rotation = -90
    end
    self.m_Velocity = vec2(0, 0)
    self.m_StateMachine:change_state("invincible")
end

function Ship:PopProjectiles()
    local projs = self.m_PendingProjectiles
    self.m_PendingProjectiles = {}
    return projs
end

function Ship:Destroy()
    self.m_Dead = true
    local ent = Entity(self.m_EntityID)
    ent:kill()
end

function Ship:IsDead()
    return self.m_Dead
end

function Ship:GetPosition()
    local ent = Entity(self.m_EntityID)
    local transform = ent:get_component(Transform)
    return transform and transform.position or vec2(320, 240)
end
