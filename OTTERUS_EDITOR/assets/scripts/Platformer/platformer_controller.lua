-- Platformer Controller
-- Orchestrates player input, physics, animation, and state management

PlatformerController = {}
PlatformerController.__index = PlatformerController

function PlatformerController:set_animation(name)
    local cfg = PlatformerAnimations[name]
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

    self.m_SpawnX    = PlatformerConfig.RESPAWN_X
    self.m_SpawnY    = PlatformerConfig.RESPAWN_Y
    self.m_IsDead    = false
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

function PlatformerController:respawn()
    if self.m_Player then
        self.m_Player:kill()
    end

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
    self.m_FlippedThisFrame = false
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

    -- Reset horizontal UV flip from previous frame
    if self.m_FlippedThisFrame and self.m_UVFrameW and self.m_UVFrameW > 0 then
        self.m_Sprite.uvs.u        = self.m_Sprite.uvs.u - self.m_UVFrameW
        self.m_Sprite.uvs.uv_width = self.m_UVFrameW
        self.m_FlippedThisFrame    = false
    elseif self.m_UVFrameW and self.m_UVFrameW > 0 and self.m_Sprite.uvs.uv_width < 0 then
        self.m_Sprite.uvs.uv_width = self.m_UVFrameW
    end

    -- Death boundary check
    local transform = self.m_Player:get_component(Transform)
    if transform and transform.position.y > PlatformerConfig.DEATH_Y and not self.m_IsDead then
        self:die()
    end

    if not self.m_IsDead then
        PlatformerPhysics.Update(self, dt)
        PlatformerInput.Update(self, dt)
    end

    if self.m_StateMachine then
        self.m_StateMachine:update(dt)
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
    if self.m_StateMachine then
        self.m_StateMachine:render()
    end
end

function PlatformerController:destroy()
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
