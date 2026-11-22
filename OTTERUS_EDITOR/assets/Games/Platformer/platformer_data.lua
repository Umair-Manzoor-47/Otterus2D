-- Platformer Data & Configurations

PlatformerConfig = {
    MOVE_SPEED           = 20.0,
    JUMP_VELOCITY        = -32.0,
    DOUBLE_JUMP_VELOCITY = -30.0,
    GRAVITY_FALL         = 10.0,
    GRAVITY_RISE         = 4.0,
    DEATH_Y              = 680.0,
    RESPAWN_X            = 200.0,
    RESPAWN_Y            = 370.0,
    COYOTE_TIME          = 0.10,
    JUMP_BUFFER          = 0.15,
    MAP_SCALE            = 2,

    -- Projectile & Ninja abilities config
    PROJECTILE_SPEED     = 520.0,
    PROJECTILE_LIFETIME  = 2.0,
    SHOOT_COOLDOWN       = 0.12,
    SHOOT_ANIM_TIME      = 0.14,
    DASH_SPEED           = 45.0,
    DASH_DURATION        = 0.16,
    DASH_COOLDOWN        = 0.60,
    WALL_SLIDE_SPEED     = 4.0,
    WALL_JUMP_VELOCITY_X = 26.0,
    WALL_JUMP_VELOCITY_Y = -30.0,
}

PlatformerAnimations = {
    idle        = { texture = "nf_idle",        frames = 11, rate = 10, loop = true },
    run         = { texture = "nf_run",         frames = 12, rate = 14, loop = true },
    jump        = { texture = "nf_jump",        frames = 1,  rate = 1,  loop = true },
    double_jump = { texture = "nf_double_jump", frames = 6,  rate = 14, loop = true },
    fall        = { texture = "nf_fall",        frames = 1,  rate = 1,  loop = true },
    hit         = { texture = "nf_hit",         frames = 7,  rate = 14, loop = true },
    shoot       = { texture = "nf_wall_jump",    frames = 5,  rate = 22, loop = false },
    wall_slide  = { texture = "nf_wall_jump",    frames = 5,  rate = 12, loop = true },
}
