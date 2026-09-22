-- =============================================================================
--  Platformer Controller
--  Ninja Frog player controller with horizontal ground collider
--  Applied Principles: KISS, DRY, YAGNI (identical behavior preserved)
-- =============================================================================

PlatformerController = {}
PlatformerController.__index = PlatformerController

-- Physics & Tuning
local MOVE_SPEED    = 25.0
local JUMP_VELOCITY = -28.0
local GRAVITY_FALL  = 15.0
local GRAVITY_RISE  = 5.0

-- Animation Definitions (DRY lookup table)
local ANIMATIONS = {
    idle = { texture = "nf_idle", frames = 11, rate = 10, loop = true },
    run  = { texture = "nf_run",  frames = 12, rate = 14, loop = true },
    jump = { texture = "nf_jump", frames = 1,  rate = 1,  loop = false },
    fall = { texture = "nf_fall", frames = 1,  rate = 1,  loop = false },
}

-- ---------------------------------------------------------------------------
-- Static Ground Collider (physics_showcase pattern)
-- ---------------------------------------------------------------------------
function PlatformerController:create_ground(x, y, width, height)
    local ground    = Entity("Ground", "")
    local collider  = ground:add_component(BoxCollider(width, height, vec2(0, 0)))
    local transform = ground:add_component(Transform(vec2(x, y), vec2(1, 1), 0))

    local attrs = PhysicsAttributes()
    attrs.type          = BodyType.STATIC
    attrs.density       = 1000.0
    attrs.friction      = 0.5
    attrs.restitution   = 0.0
    attrs.gravityScale  = 1.0
    attrs.position      = transform.position
    attrs.scale         = transform.scale
    attrs.boxShape      = true
    attrs.boxSize       = vec2(collider.width, collider.height)
    attrs.fixedRotation = false

    ground:add_component(PhysicsComponent(attrs))
end

-- ---------------------------------------------------------------------------
-- Player Entity Creation
-- ---------------------------------------------------------------------------
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

-- ---------------------------------------------------------------------------
-- Initialization
-- ---------------------------------------------------------------------------
function PlatformerController:init()
    self:create_ground(0, 464, 10000, 16)

    self.m_Player, self.m_Physics, self.m_Sprite, self.m_Anim = self:create_player(320, 350)
    self.m_UVFrameW     = self.m_Sprite.uvs.uv_width
    self.m_State        = "idle"
    self.m_Facing       = "right"
    self.m_Grounded     = true
    self.m_JumpCooldown = 0.0

    self.m_Camera = FollowCamera(
        FollowCamParams({ scale = 1, max_x = 20000, max_y = 2000, springback = 2.0 }),
        self.m_Player
    )
end

-- ---------------------------------------------------------------------------
-- Animation State Machine (DRY: uses ANIMATIONS table)
-- ---------------------------------------------------------------------------
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

    self.m_UVFrameW = self.m_Sprite.uvs.uv_width
end

-- ---------------------------------------------------------------------------
-- Frame Update
-- ---------------------------------------------------------------------------
function PlatformerController:update(dt)
    dt = dt or 0.016

    -- Restore positive uv_width before AnimationSystem updates
    if self.m_UVFrameW and self.m_UVFrameW > 0 then
        self.m_Sprite.uvs.uv_width = self.m_UVFrameW
    end

    local velocity = self.m_Physics:get_linear_velocity()

    -- 1. Dynamic Gravity Scaling (rising vs falling)
    self.m_Physics:set_gravity_scale(velocity.y > 0.0 and GRAVITY_FALL or GRAVITY_RISE)

    -- 2. Grounded Check & Jump Cooldown
    if math.abs(velocity.y) < 0.2 then
        self.m_Grounded = true
    elseif math.abs(velocity.y) > 1.0 then
        self.m_Grounded = false
    end

    if self.m_JumpCooldown > 0 then
        self.m_JumpCooldown = self.m_JumpCooldown - dt
    end

    -- 3. Horizontal Input (Arrow Keys & WASD)
    local moveX = 0
    if Keyboard.pressed(KEY_D) or Keyboard.pressed(KEY_RIGHT) then
        moveX = MOVE_SPEED
        self.m_Facing = "right"
    elseif Keyboard.pressed(KEY_A) or Keyboard.pressed(KEY_LEFT) then
        moveX = -MOVE_SPEED
        self.m_Facing = "left"
    end

    -- 4. Jump Input (Tap or Hold)
    local jumpKeyHeld        = Keyboard.pressed(KEY_W) or Keyboard.pressed(KEY_UP) or Keyboard.pressed(KEY_SPACE)
    local jumpKeyJustPressed = Keyboard.just_pressed(KEY_W) or Keyboard.just_pressed(KEY_UP) or Keyboard.just_pressed(KEY_SPACE)
    local wantsJump          = jumpKeyJustPressed or (self.m_Grounded and jumpKeyHeld and self.m_JumpCooldown <= 0)

    if wantsJump and self.m_Grounded then
        self.m_Physics:set_linear_velocity(vec2(moveX, JUMP_VELOCITY))
        self.m_Grounded     = false
        self.m_JumpCooldown = 0.2
    else
        self.m_Physics:set_linear_velocity(vec2(moveX, velocity.y))
    end

    -- Re-read velocity after movement/jump
    velocity = self.m_Physics:get_linear_velocity()

    -- 5. Determine Animation State
    if not self.m_Grounded then
        self:set_state(velocity.y < 0 and "jump" or "fall")
    elseif moveX ~= 0 then
        self:set_state("run")
    else
        self:set_state("idle")
    end

    -- 6. Follow Camera
    if self.m_Camera then
        self.m_Camera:update()
    end
end

-- ---------------------------------------------------------------------------
-- Render Hook (flips UVs right before RenderSystem, keeping collider aligned)
-- ---------------------------------------------------------------------------
function PlatformerController:render()
    if self.m_Facing == "left" and self.m_UVFrameW and self.m_UVFrameW > 0 then
        self.m_Sprite.uvs.u        = self.m_Sprite.uvs.u + self.m_UVFrameW
        self.m_Sprite.uvs.uv_width = -self.m_UVFrameW
    end
end

-- ---------------------------------------------------------------------------
-- Factory
-- ---------------------------------------------------------------------------
function PlatformerController:Create()
    local this = setmetatable({}, PlatformerController)
    this:init()
    return this
end
