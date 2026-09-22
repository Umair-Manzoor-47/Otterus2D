run_script("assets/scripts/Platformer/platformerMap.lua")
run_script("assets/scripts/Platformer/map_loader.lua")

PlatformerController = {}
PlatformerController.__index = PlatformerController

local MOVE_SPEED           = 20.0
local JUMP_VELOCITY        = -32.0
local DOUBLE_JUMP_VELOCITY = -30.0
local GRAVITY_FALL         = 10.0
local GRAVITY_RISE         = 4.0
local DEATH_Y              = 680.0
local RESPAWN_X            = 200.0
local RESPAWN_Y            = 370.0

local ANIMATIONS = {
    idle        = { texture = "nf_idle",        frames = 11, rate = 10, loop = true },
    run         = { texture = "nf_run",         frames = 12, rate = 14, loop = true },
    jump        = { texture = "nf_jump",        frames = 1,  rate = 1,  loop = true },
    double_jump = { texture = "nf_double_jump", frames = 6,  rate = 14, loop = true },
    fall        = { texture = "nf_fall",        frames = 1,  rate = 1,  loop = true },
    hit         = { texture = "nf_hit",         frames = 7,  rate = 14, loop = true },
}

function PlatformerController:create_player(x, y)
    local player    = Entity("NinjaFrog", "player")
    local collider  = player:add_component(BoxCollider(32, 32, vec2(0, 0)))
    local transform = player:add_component(Transform(vec2(x, y), vec2(1, 1), 0))

    local attrs = PhysicsAttributes()
    attrs.type          = BodyType.DYNAMIC
    attrs.density       = 100.0
    attrs.friction      = 0.2
    attrs.restitution   = 0.0
    attrs.gravityScale  = 2.0
    attrs.position      = transform.position
    attrs.scale         = transform.scale
    attrs.boxShape      = true
    attrs.boxSize       = vec2(collider.width, collider.height)
    attrs.fixedRotation = true

    local physics = player:add_component(PhysicsComponent(attrs))
    local sprite  = player:add_component(Sprite("nf_idle", 32, 32, 0, 0, 1))
    sprite:generate_uvs()

    local anim = player:add_component(Animation(11, 10, 0, false, true))

    return player, physics, sprite, anim
end

function PlatformerController:init()
    local tilemap = CreatePlatformerMap()
    assert(tilemap, "Failed to create platformer map")
    self.m_Map = PlatformerMapLoader:Load(tilemap)

    self.m_SpawnX = RESPAWN_X
    self.m_SpawnY = RESPAWN_Y
    self.m_IsDead = false
    self.m_DeathTimer = 0.0

    self.m_Player, self.m_Physics, self.m_Sprite, self.m_Anim = self:create_player(self.m_SpawnX, self.m_SpawnY)
    self.m_UVFrameW         = self.m_Sprite.uvs.uv_width
    self.m_State            = "idle"
    self.m_Facing           = "right"
    self.m_Grounded         = true
    self.m_IsJumping        = false
    self.m_IsDoubleJumping  = false
    self.m_CanDoubleJump    = true
    self.m_JumpBuffer       = 0.0
    self.m_CoyoteTimer      = 0.10
    self.m_FlippedThisFrame = false

    self.m_Camera = FollowCamera(
        FollowCamParams({
            scale      = 1,
            min_x      = 0,
            min_y      = 0,
            max_x      = self.m_Map.pixel_width,
            max_y      = self.m_Map.pixel_height,
            springback = 2.0
        }),
        self.m_Player
    )
end

function PlatformerController:die()
    if self.m_IsDead then return end
    self.m_IsDead = true
    self.m_DeathTimer = 0.5
    self:set_state("hit")
    self.m_Physics:set_linear_velocity(vec2(0, -12.0))
end

function PlatformerController:respawn()
    if self.m_Player then
        self.m_Player:kill()
    end

    self.m_Player, self.m_Physics, self.m_Sprite, self.m_Anim = self:create_player(self.m_SpawnX, self.m_SpawnY)
    self.m_Camera:set_entity(self.m_Player)

    self.m_UVFrameW         = self.m_Sprite.uvs.uv_width
    self.m_State            = "idle"
    self.m_Facing           = "right"
    self.m_Grounded         = true
    self.m_IsJumping        = false
    self.m_IsDoubleJumping  = false
    self.m_CanDoubleJump    = true
    self.m_JumpBuffer       = 0.0
    self.m_CoyoteTimer      = 0.10
    self.m_IsDead           = false
    self.m_DeathTimer       = 0.0
    self.m_FlippedThisFrame = false
end

function PlatformerController:set_state(state)
    if self.m_State == state then return end
    self.m_State = state

    local cfg = ANIMATIONS[state]
    if not cfg then return end

    self.m_Sprite.texture_name = cfg.texture
    self.m_Sprite.start_x      = 0
    self.m_Sprite.start_y      = 0
    self.m_Sprite:generate_uvs()

    self.m_Anim.num_frames = cfg.frames
    self.m_Anim.frame_rate = cfg.rate
    self.m_Anim.looped     = cfg.loop
    self.m_Anim:reset()

    self.m_UVFrameW         = self.m_Sprite.uvs.uv_width
    self.m_FlippedThisFrame = false
end

function PlatformerController:update(dt)
    dt = dt or 0.016

    if Keyboard.just_pressed(KEY_C) or Keyboard.just_pressed(KEY_F1) then
        if IsRenderCollidersEnabled() then
            DisableRenderColliders()
        else
            EnableRenderColliders()
        end
    end

    if Keyboard.just_pressed(KEY_R) and not self.m_IsDead then
        self:die()
    end

    if self.m_FlippedThisFrame and self.m_UVFrameW and self.m_UVFrameW > 0 then
        self.m_Sprite.uvs.u        = self.m_Sprite.uvs.u - self.m_UVFrameW
        self.m_Sprite.uvs.uv_width = self.m_UVFrameW
        self.m_FlippedThisFrame    = false
    elseif self.m_UVFrameW and self.m_UVFrameW > 0 and self.m_Sprite.uvs.uv_width < 0 then
        self.m_Sprite.uvs.uv_width = self.m_UVFrameW
    end

    local transform = self.m_Player:get_component(Transform)

    if self.m_IsDead then
        self.m_DeathTimer = self.m_DeathTimer - dt
        if self.m_DeathTimer <= 0 then
            self:respawn()
        end
        if self.m_Camera then self.m_Camera:update() end
        return
    end

    if transform and transform.position.y > DEATH_Y then
        self:die()
        return
    end

    local velocity = self.m_Physics:get_linear_velocity()
    local vy = velocity.y

    self.m_Physics:set_gravity_scale(vy > 0.0 and GRAVITY_FALL or GRAVITY_RISE)

    if vy > 1.0 or vy < -0.5 then
        self.m_Grounded = false
    elseif math.abs(vy) <= 0.2 and not self.m_IsJumping then
        self.m_Grounded        = true
        self.m_CanDoubleJump   = true
        self.m_IsDoubleJumping = false
    end

    if self.m_IsJumping and vy >= 0.0 and math.abs(vy) <= 0.2 then
        self.m_IsJumping       = false
        self.m_IsDoubleJumping = false
        self.m_CanDoubleJump   = true
        self.m_Grounded        = true
    end

    if self.m_Grounded then
        self.m_CoyoteTimer = 0.10
    elseif self.m_CoyoteTimer > 0 then
        self.m_CoyoteTimer = self.m_CoyoteTimer - dt
    end

    local moveX = 0
    if Keyboard.pressed(KEY_D) or Keyboard.pressed(KEY_RIGHT) then
        moveX = MOVE_SPEED
        self.m_Facing = "right"
    elseif Keyboard.pressed(KEY_A) or Keyboard.pressed(KEY_LEFT) then
        moveX = -MOVE_SPEED
        self.m_Facing = "left"
    end

    local jumpJustPressed = Keyboard.just_pressed(KEY_W)
                         or Keyboard.just_pressed(KEY_UP)
                         or Keyboard.just_pressed(KEY_SPACE)

    if jumpJustPressed then
        self.m_JumpBuffer = 0.15
    elseif self.m_JumpBuffer > 0 then
        self.m_JumpBuffer = self.m_JumpBuffer - dt
    end

    if self.m_JumpBuffer > 0 and self.m_CoyoteTimer > 0 and not self.m_IsJumping then
        self.m_Physics:set_linear_velocity(vec2(moveX, JUMP_VELOCITY))
        self.m_Grounded        = false
        self.m_IsJumping       = true
        self.m_IsDoubleJumping = false
        self.m_CanDoubleJump   = true
        self.m_CoyoteTimer     = 0.0
        self.m_JumpBuffer      = 0.0
    elseif jumpJustPressed and not self.m_Grounded and self.m_CanDoubleJump then
        self.m_Physics:set_linear_velocity(vec2(moveX, DOUBLE_JUMP_VELOCITY))
        self.m_Grounded        = false
        self.m_IsJumping       = true
        self.m_IsDoubleJumping = true
        self.m_CanDoubleJump   = false
        self.m_JumpBuffer      = 0.0
        self:set_state("double_jump")
    else
        self.m_Physics:set_linear_velocity(vec2(moveX, vy))
    end

    local jumpReleased = Keyboard.just_released(KEY_W)
                      or Keyboard.just_released(KEY_UP)
                      or Keyboard.just_released(KEY_SPACE)

    if jumpReleased and velocity.y < -5.0 then
        self.m_Physics:set_linear_velocity(vec2(moveX, velocity.y * 0.45))
    end

    velocity = self.m_Physics:get_linear_velocity()

    if not self.m_Grounded then
        if self.m_IsDoubleJumping and velocity.y < 0 then
            self:set_state("double_jump")
        elseif velocity.y < 0 then
            self:set_state("jump")
        else
            self:set_state("fall")
        end
    elseif moveX ~= 0 then
        self:set_state("run")
    else
        self:set_state("idle")
    end

    if self.m_Camera then
        self.m_Camera:update()
    end
end

function PlatformerController:render()
    if self.m_Facing == "left" and self.m_UVFrameW and self.m_UVFrameW > 0 and not self.m_FlippedThisFrame then
        self.m_Sprite.uvs.u        = self.m_Sprite.uvs.u + self.m_UVFrameW
        self.m_Sprite.uvs.uv_width = -self.m_UVFrameW
        self.m_FlippedThisFrame    = true
    end
end

function PlatformerController:Create()
    local this = setmetatable({}, PlatformerController)
    this:init()
    return this
end
