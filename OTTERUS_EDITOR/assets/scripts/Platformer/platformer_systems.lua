-- Platformer Systems
-- Decoupled input and physics logic for character movement

PlatformerInput = {}

function PlatformerInput.Update(controller, dt)
    controller.m_JumpJustPressed = Keyboard.just_pressed(KEY_W)
                                or Keyboard.just_pressed(KEY_UP)
                                or Keyboard.just_pressed(KEY_SPACE)

    controller.m_JumpJustReleased = Keyboard.just_released(KEY_W)
                                 or Keyboard.just_released(KEY_UP)
                                 or Keyboard.just_released(KEY_SPACE)

    if controller.m_JumpJustPressed then
        controller.m_JumpBuffer = PlatformerConfig.JUMP_BUFFER
    elseif controller.m_JumpBuffer > 0 then
        controller.m_JumpBuffer = controller.m_JumpBuffer - dt
    end

    if controller.m_Grounded then
        controller.m_CoyoteTimer = PlatformerConfig.COYOTE_TIME
    elseif controller.m_CoyoteTimer > 0 then
        controller.m_CoyoteTimer = controller.m_CoyoteTimer - dt
    end
end

function PlatformerInput.GetHorizontalMove(controller)
    local moveX = 0
    if Keyboard.pressed(KEY_D) or Keyboard.pressed(KEY_RIGHT) then
        moveX = PlatformerConfig.MOVE_SPEED
        controller.m_Facing = "right"
    elseif Keyboard.pressed(KEY_A) or Keyboard.pressed(KEY_LEFT) then
        moveX = -PlatformerConfig.MOVE_SPEED
        controller.m_Facing = "left"
    end
    return moveX
end

function PlatformerInput.CheckGroundJump(controller)
    if controller.m_JumpBuffer > 0 and controller.m_CoyoteTimer > 0 then
        controller:change_state("jump")
        return true
    end
    return false
end

function PlatformerInput.ApplyJumpCut(controller, moveX, vy)
    if controller.m_JumpJustReleased and vy < -5.0 then
        controller.m_Physics:set_linear_velocity(vec2(moveX, vy * 0.45))
    end
end

PlatformerPhysics = {}

function PlatformerPhysics.Update(controller, dt)
    local velocity = controller.m_Physics:get_linear_velocity()
    local vy = velocity.y

    -- Dynamic gravity scale (faster falling for crisp platforming feel)
    local grav = (vy > 0.0) and PlatformerConfig.GRAVITY_FALL or PlatformerConfig.GRAVITY_RISE
    controller.m_Physics:set_gravity_scale(grav)

    -- Ground detection heuristic based on vertical speed
    if vy > 1.0 or vy < -0.5 then
        controller.m_Grounded = false
    elseif math.abs(vy) <= 0.2 and not controller.m_IsJumping then
        controller.m_Grounded        = true
        controller.m_CanDoubleJump   = true
        controller.m_IsDoubleJumping = false
    end

    if controller.m_IsJumping and vy >= 0.0 and math.abs(vy) <= 0.2 then
        controller.m_IsJumping       = false
        controller.m_IsDoubleJumping = false
        controller.m_CanDoubleJump   = true
        controller.m_Grounded        = true
    end
end
