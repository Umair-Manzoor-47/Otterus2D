-- Platformer Controller
-- Orchestrates player input, physics, animation, projectile combat, and state management

PlatformerController = {}
PlatformerController.__index = PlatformerController

function PlatformerController:set_animation(name, force)
    -- Don't interrupt an active shoot animation unless forced or dying
    if self.m_ShootAnimTimer and self.m_ShootAnimTimer > 0 and not force and name ~= "shoot" and name ~= "hit" then
        return
    end

    local cfg = PlatformerAnimations[name]
    if not cfg then return end

    -- When force is true, we intentionally allow restarting the animation (e.g. repeated rapid throws)
    if not force and self.m_CurrentAnim == name and self.m_Sprite.texture_name == cfg.texture then
        return
    end

    self.m_CurrentAnim         = name
    self.m_AnimTimer           = 0.0
    self.m_Sprite.texture_name = cfg.texture
    self.m_Sprite.start_x      = 0
    self.m_Sprite.start_y      = 0
    self.m_Sprite:generate_uvs()

    self.m_UVFrameW = self.m_Sprite.uvs.uv_width

    -- Set C++ AnimationComponent num_frames to 0 so the engine AnimationSystem
    -- does not overwrite our horizontal flip UVs.
    if self.m_Anim then
        self.m_Anim.num_frames = 0
    end

    self:update_animation(0)
end

function PlatformerController:update_animation(dt)
    local cfg = PlatformerAnimations[self.m_CurrentAnim]
    if not cfg or not self.m_UVFrameW or self.m_UVFrameW <= 0 then return end

    self.m_AnimTimer = self.m_AnimTimer + dt
    local frame = 0
    if cfg.frames > 1 and cfg.rate > 0 then
        if cfg.loop then
            frame = math.floor(self.m_AnimTimer * cfg.rate) % cfg.frames
        else
            frame = math.min(math.floor(self.m_AnimTimer * cfg.rate), cfg.frames - 1)
        end
    end

    local frameW = self.m_UVFrameW
    -- In Otterus2D, setting negative uv_width mirrors the quad horizontally,
    -- with U starting at (frame + 1) * frameW
    if self.m_Facing == "left" then
        self.m_Sprite.uvs.u        = (frame + 1) * frameW
        self.m_Sprite.uvs.uv_width = -frameW
    else
        self.m_Sprite.uvs.u        = frame * frameW
        self.m_Sprite.uvs.uv_width = frameW
    end
end

function PlatformerController:change_state(name)
    if self.m_StateMachine:current_state() == name then
        return
    end
    self.m_StateMachine:change_state(name)
end

function PlatformerController:set_state(state)
    self:change_state(state)
end

function PlatformerController:init()
    local tilemap = CreatePlatformerMap()
    assert(tilemap, "Failed to create platformer map")
    self.m_Map = MapLoader:Load(tilemap, { scale = PlatformerConfig.MAP_SCALE })

    self.m_SpawnX     = PlatformerConfig.RESPAWN_X
    self.m_SpawnY     = PlatformerConfig.RESPAWN_Y
    self.m_IsDead     = false
    self.m_DeathTimer = 0.0

    self.m_Player, self.m_Physics, self.m_Sprite, self.m_Anim = PlayerFactory.CreatePlayer(self.m_SpawnX, self.m_SpawnY)
    self.m_UVFrameW         = self.m_Sprite.uvs.uv_width
    self.m_Facing           = "right"
    self.m_Grounded         = true
    self.m_IsJumping        = false
    self.m_IsDoubleJumping  = false
    self.m_CanDoubleJump    = true
    self.m_JumpBuffer       = 0.0
    self.m_CoyoteTimer      = PlatformerConfig.COYOTE_TIME
    self.m_CurrentAnim      = "idle"
    self.m_AnimTimer        = 0.0

    -- Combat & abilities state
    self.m_ShootCooldown    = 0.0
    self.m_ShootAnimTimer   = 0.0
    self.m_IsDashing        = false
    self.m_DashTimer        = 0.0
    self.m_DashCooldown     = 0.0
    self.m_IsWallSliding    = false

    -- Disable C++ AnimationComponent override so Lua controls UVs
    if self.m_Anim then
        self.m_Anim.num_frames = 0
    end

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

    self.m_StateMachine = StateMachine()
    self.m_States = PlayerStates.Create(self)
    for _, state in pairs(self.m_States) do
        self.m_StateMachine:add_state(state)
    end
    self:change_state("idle")

    self.m_Deaths = 0
    self.m_UIEntity, self.m_UIText = PlayerFactory.CreateUI()
end

function PlatformerController:set_ui_visible(visible)
    if self.m_UIText then
        self.m_UIText.isHidden = not visible
    end
end

function PlatformerController:reset_deaths()
    self.m_Deaths = 0
    if self.m_UIText then
        self.m_UIText.textStr = "Deaths: 0"
    end
end

function PlatformerController:die()
    if self.m_IsDead or self.m_StateMachine:current_state() == "hit" then return end
    self.m_Deaths = (self.m_Deaths or 0) + 1
    if self.m_UIText then
        self.m_UIText.textStr = "Deaths: " .. tostring(self.m_Deaths)
    end
    self:change_state("hit")
end

function PlatformerController:shoot()
    self.m_ShootCooldown  = PlatformerConfig.SHOOT_COOLDOWN
    self.m_ShootAnimTimer = PlatformerConfig.SHOOT_ANIM_TIME

    local tf = self.m_Player:get_component(Transform)
    if tf then
        local dirX = (self.m_Facing == "left") and -1 or 1
        local spawnX = tf.position.x + (dirX > 0 and 26 or -10)
        local spawnY = tf.position.y + 8

        Projectile.Spawn({
            x         = spawnX,
            y         = spawnY,
            dirX      = dirX,
            speed     = PlatformerConfig.PROJECTILE_SPEED,
            lifetime  = PlatformerConfig.PROJECTILE_LIFETIME,
            spinSpeed = 900.0,
            texture   = "ball",
            scale     = 0.55,
            layer     = 3
        })
    end

    -- Play shoot animation (5 frames of throwing/attack pose)
    self:set_animation("shoot", true)

    if AssetLoader and AssetLoader.PlaySound then
        AssetLoader.PlaySound("laser")
    end
end

function PlatformerController:dash()
    self.m_IsDashing    = true
    self.m_DashTimer    = PlatformerConfig.DASH_DURATION
    self.m_DashCooldown = PlatformerConfig.DASH_COOLDOWN
    local dashDir = (self.m_Facing == "left") and -1 or 1
    self.m_Physics:set_linear_velocity(vec2(dashDir * PlatformerConfig.DASH_SPEED, 0))
    self:set_animation("shoot", true)

    if AssetLoader and AssetLoader.PlaySound then
        AssetLoader.PlaySound("leaves_rustle")
    end
end

function PlatformerController:respawn()
    if self.m_Player then
        self.m_Player:kill()
    end

    Projectile.ClearAll()

    self.m_Player, self.m_Physics, self.m_Sprite, self.m_Anim = PlayerFactory.CreatePlayer(self.m_SpawnX, self.m_SpawnY)
    self.m_Camera:set_entity(self.m_Player)

    self.m_UVFrameW         = self.m_Sprite.uvs.uv_width
    self.m_Facing           = "right"
    self.m_Grounded         = true
    self.m_IsJumping        = false
    self.m_IsDoubleJumping  = false
    self.m_CanDoubleJump    = true
    self.m_JumpBuffer       = 0.0
    self.m_CoyoteTimer      = PlatformerConfig.COYOTE_TIME
    self.m_IsDead           = false
    self.m_DeathTimer       = 0.0
    self.m_CurrentAnim      = "idle"
    self.m_AnimTimer        = 0.0
    self.m_ShootCooldown    = 0.0
    self.m_ShootAnimTimer   = 0.0
    self.m_IsDashing        = false
    self.m_DashTimer        = 0.0
    self.m_DashCooldown     = 0.0
    self.m_IsWallSliding    = false

    if self.m_Anim then
        self.m_Anim.num_frames = 0
    end

    self:set_animation("idle", true)
end

function PlatformerController:update(dt)
    dt = dt or 0.016

    -- Toggle collider debug
    if Keyboard.just_pressed(KEY_C) or Keyboard.just_pressed(KEY_F1) then
        if IsRenderCollidersEnabled() then
            DisableRenderColliders()
        else
            EnableRenderColliders()
        end
    end

    -- Manual respawn hotkey
    if Keyboard.just_pressed(KEY_R) and not self.m_IsDead then
        self:die()
    end

    -- Death boundary check
    local transform = self.m_Player:get_component(Transform)
    if transform and transform.position.y > PlatformerConfig.DEATH_Y and not self.m_IsDead then
        self:die()
    end

    if not self.m_IsDead then
        -- Decrement combat / ability timers
        if self.m_ShootCooldown > 0 then
            self.m_ShootCooldown = self.m_ShootCooldown - dt
        end

        if self.m_ShootAnimTimer > 0 then
            self.m_ShootAnimTimer = self.m_ShootAnimTimer - dt
            if self.m_ShootAnimTimer <= 0 and not self.m_IsDashing then
                local curState = self.m_StateMachine:current_state()
                self:set_animation(curState or "idle", true)
            end
        end

        if self.m_DashCooldown > 0 then
            self.m_DashCooldown = self.m_DashCooldown - dt
        end

        if self.m_IsDashing then
            self.m_DashTimer = self.m_DashTimer - dt
            local dashDir = (self.m_Facing == "left") and -1 or 1
            self.m_Physics:set_linear_velocity(vec2(dashDir * PlatformerConfig.DASH_SPEED, 0))
            if self.m_DashTimer <= 0 then
                self.m_IsDashing = false
                self.m_Physics:set_linear_velocity(vec2(dashDir * PlatformerConfig.MOVE_SPEED, 0))
                local curState = self.m_StateMachine:current_state()
                self:set_animation(curState or "idle", true)
            end
        end

        PlatformerPhysics.Update(self, dt)
        PlatformerInput.Update(self, dt)

        -- Handle Projectile Shoot (SPACE key)
        if self.m_ShootJustPressed and self.m_ShootCooldown <= 0 then
            self:shoot()
        end

        -- Handle Ninja Dash (Shift key)
        if self.m_DashJustPressed and not self.m_IsDashing and self.m_DashCooldown <= 0 then
            self:dash()
        end
    end

    if self.m_StateMachine then
        self.m_StateMachine:update(dt)
    end

    if self.m_Camera then
        self.m_Camera:update()
    end

    -- Update all active projectiles
    Projectile.UpdateAll(dt)

    -- Update player animation and horizontal flip
    if self.m_Player and not self.m_IsDead then
        self:update_animation(dt)
    end
end

function PlatformerController:render()
    if self.m_StateMachine then
        self.m_StateMachine:render()
    end
end

function PlatformerController:destroy()
    Projectile.ClearAll()

    if self.m_Map and self.m_Map.destroy then
        self.m_Map:destroy()
        self.m_Map = nil
    end
    if self.m_Player then
        self.m_Player:kill()
        self.m_Player = nil
    end
    if self.m_UIEntity then
        self.m_UIEntity:kill()
        self.m_UIEntity = nil
    end
end

function PlatformerController:Create()
    local this = setmetatable({}, PlatformerController)
    this:init()
    return this
end
