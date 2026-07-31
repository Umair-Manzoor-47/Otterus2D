Ship = {}
Ship.__index = Ship

CooldownTimer = Timer()
CooldownTime = 300

function Ship:Create(def)
    local this = {
        m_EntityID = def.id,

        m_FwdSpeed = def.fwd_speed or 5,
        m_AngSpeed = def.ang_speed or 2,

        -- Movement
        m_Velocity = vec2(0, 0),
        m_Acceleration = 0.2,

        m_Drag = 0.98,

        -- Maximum movement speed
        m_MaxSpeed = 7,
    }

    CooldownTimer:start()

    setmetatable(this, self)

    return this
end

function Ship:UpdateShip()
    local ship = Entity(self.m_EntityID)

    -- Components
    local transform = ship:get_component(Transform)
    local sprite = ship:get_component(Sprite)

    if not transform then
        print("Ship has no Transform component")
        return
    end

    if not sprite then
        print("Ship has no Sprite component")
        return
    end

    -- Forward direction
    local fwd = vec2(
        math.cos(math.rad(transform.rotation)),
        math.sin(math.rad(transform.rotation))
    )

    -- Rotation
    if Keyboard.pressed(KEY_A) then
        transform.rotation = transform.rotation - self.m_AngSpeed
    elseif Keyboard.pressed(KEY_D) then
        transform.rotation = transform.rotation + self.m_AngSpeed
    end

    -- Thrust
    if Keyboard.pressed(KEY_W) then
        self.m_Velocity =
            self.m_Velocity +
            fwd * self.m_Acceleration
    end

    -- Apply drag
    self.m_Velocity =
        self.m_Velocity * self.m_Drag

    -- Move ship
    transform.position =
        transform.position +
        self.m_Velocity

    -- Shoot projectile
    if Keyboard.just_pressed(KEY_SPACE) and self:CooldownEnded() then
        local projectile = Projectile:Create(
            {
                def = "proj_1",
                dir = fwd,
                start_pos = vec2(
                        transform.position.x + sprite.width / 2,
                        transform.position.y + sprite.height / 2
                ),
                rotation = transform.rotation

            }
        )
        AddProjectile( projectile )
        CooldownTimer:restart()

    end


    CheckPosition(transform.position, sprite.width, sprite.height)
end

function Ship:CooldownEnded()
    return CooldownTimer:elapsed_ms() >= CooldownTime
end