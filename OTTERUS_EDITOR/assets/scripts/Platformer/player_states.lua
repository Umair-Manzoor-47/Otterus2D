-- Player States
-- Defines player Finite State Machine states

PlayerStates = {}

function PlayerStates.Create(controller)
    local function make_state(name, on_enter, on_update, on_exit)
        local state = State(name)
        state:set_on_enter(function()
            controller:set_animation(name)
            if on_enter then on_enter(controller) end
        end)
        if on_update then
            state:set_on_update(function(dt)
                on_update(controller, dt)
            end)
        end
        if on_exit then
            state:set_on_exit(function()
                on_exit(controller)
            end)
        end
        return state
    end

    local states = {}

    -- Idle State
    states.idle = make_state("idle",
        function(this)
            this.m_Grounded        = true
            this.m_CanDoubleJump   = true
            this.m_IsJumping       = false
            this.m_IsDoubleJumping = false
            local velocity = this.m_Physics:get_linear_velocity()
            this.m_Physics:set_linear_velocity(vec2(0, velocity.y))
        end,
        function(this, dt)
            if not this.m_Grounded then
                this:change_state("fall")
                return
            end
            if PlatformerInput.CheckGroundJump(this) then
                return
            end
            local moveX = PlatformerInput.GetHorizontalMove(this)
            if moveX ~= 0 then
                this:change_state("run")
                return
            end
            local velocity = this.m_Physics:get_linear_velocity()
            this.m_Physics:set_linear_velocity(vec2(0, velocity.y))
        end
    )

    -- Run State
    states.run = make_state("run",
        function(this)
            this.m_Grounded        = true
            this.m_CanDoubleJump   = true
            this.m_IsJumping       = false
            this.m_IsDoubleJumping = false
        end,
        function(this, dt)
            if not this.m_Grounded then
                this:change_state("fall")
                return
            end
            if PlatformerInput.CheckGroundJump(this) then
                return
            end
            local moveX = PlatformerInput.GetHorizontalMove(this)
            if moveX == 0 then
                this:change_state("idle")
                return
            end
            local velocity = this.m_Physics:get_linear_velocity()
            this.m_Physics:set_linear_velocity(vec2(moveX, velocity.y))
        end
    )

    -- Jump State
    states.jump = make_state("jump",
        function(this)
            local moveX = PlatformerInput.GetHorizontalMove(this)
            this.m_Physics:set_linear_velocity(vec2(moveX, PlatformerConfig.JUMP_VELOCITY))
            this.m_Grounded        = false
            this.m_IsJumping       = true
            this.m_IsDoubleJumping = false
            this.m_CanDoubleJump   = true
            this.m_CoyoteTimer     = 0.0
            this.m_JumpBuffer      = 0.0
        end,
        function(this, dt)
            local moveX = PlatformerInput.GetHorizontalMove(this)
            local velocity = this.m_Physics:get_linear_velocity()
            PlatformerInput.ApplyJumpCut(this, moveX, velocity.y)
            velocity = this.m_Physics:get_linear_velocity()

            if this.m_JumpJustPressed and this.m_CanDoubleJump then
                this:change_state("double_jump")
                return
            end
            if velocity.y >= 0.0 then
                this:change_state("fall")
                return
            end
            this.m_Physics:set_linear_velocity(vec2(moveX, velocity.y))
        end
    )

    -- Double Jump State
    states.double_jump = make_state("double_jump",
        function(this)
            local moveX = PlatformerInput.GetHorizontalMove(this)
            this.m_Physics:set_linear_velocity(vec2(moveX, PlatformerConfig.DOUBLE_JUMP_VELOCITY))
            this.m_Grounded        = false
            this.m_IsJumping       = true
            this.m_IsDoubleJumping = true
            this.m_CanDoubleJump   = false
            this.m_JumpBuffer      = 0.0
        end,
        function(this, dt)
            local moveX = PlatformerInput.GetHorizontalMove(this)
            local velocity = this.m_Physics:get_linear_velocity()
            PlatformerInput.ApplyJumpCut(this, moveX, velocity.y)
            velocity = this.m_Physics:get_linear_velocity()

            if velocity.y >= 0.0 then
                this:change_state("fall")
                return
            end
            this.m_Physics:set_linear_velocity(vec2(moveX, velocity.y))
        end
    )

    -- Fall State
    states.fall = make_state("fall",
        function(this)
            this.m_Grounded = false
        end,
        function(this, dt)
            local moveX = PlatformerInput.GetHorizontalMove(this)
            local velocity = this.m_Physics:get_linear_velocity()

            if this.m_JumpBuffer > 0 and this.m_CoyoteTimer > 0 and not this.m_IsJumping then
                this:change_state("jump")
                return
            end
            if this.m_JumpJustPressed and this.m_CanDoubleJump then
                this:change_state("double_jump")
                return
            end
            if this.m_Grounded then
                if this.m_JumpBuffer > 0 then
                    this:change_state("jump")
                elseif moveX ~= 0 then
                    this:change_state("run")
                else
                    this:change_state("idle")
                end
                return
            end
            this.m_Physics:set_linear_velocity(vec2(moveX, velocity.y))
        end
    )

    -- Hit / Death State
    states.hit = make_state("hit",
        function(this)
            this.m_IsDead = true
            this.m_DeathTimer = 0.5
            this.m_Physics:set_linear_velocity(vec2(0, -12.0))
        end,
        function(this, dt)
            this.m_DeathTimer = this.m_DeathTimer - dt
            if this.m_DeathTimer <= 0 then
                this:respawn()
                this:change_state("idle")
            end
        end,
        function(this)
            this.m_IsDead = false
        end
    )

    return states
end
