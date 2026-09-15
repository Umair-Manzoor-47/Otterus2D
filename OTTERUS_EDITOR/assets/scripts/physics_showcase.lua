PhysicsShowcase = {
    window_width = 640,
    window_height = 480,
    wall_thickness = 16,
    ball_count = 0,
    ball = nil,
    camera = nil,
    val_text = nil,
    walls = {}
}

function PhysicsShowcase:create_wall(name, width, height, x, y)
    local wall = Entity(name, "")
    local collider = wall:add_component(BoxCollider(width, height, vec2(0, 0)))
    local transform = wall:add_component(Transform(vec2(x, y), vec2(1, 1), 0))

    local physAttrs = PhysicsAttributes()
    physAttrs.type = BodyType.STATIC
    physAttrs.density = 1000.0
    physAttrs.friction = 0.5
    physAttrs.restitution = 0.0
    physAttrs.gravityScale = 1.0
    physAttrs.position = transform.position
    physAttrs.scale = transform.scale
    physAttrs.boxShape = true
    physAttrs.boxSize = vec2(collider.width, collider.height)
    physAttrs.fixedRotation = false

    local physics = wall:add_component(PhysicsComponent(physAttrs))

    return wall, collider, transform, physics
end

function PhysicsShowcase:create_ball(pos_x, pos_y, scale_x, scale_y)
    local ball = Entity("", "")
    local circle = ball:add_component(CircleCollider(16.0))
    local transform = ball:add_component(Transform(vec2(pos_x, pos_y), vec2(scale_x, scale_y), 0))
    local physAttribs = PhysicsAttributes()

    physAttribs.type = BodyType.DYNAMIC
    physAttribs.density = 100.0
    physAttribs.friction = 0.2
    physAttribs.restitution = 0.1
    physAttribs.radius = circle.radius
    physAttribs.gravityScale = 2.0
    physAttribs.position = transform.position
    physAttribs.scale = transform.scale
    physAttribs.circle = true
    physAttribs.fixedRotation = false

    local physics = ball:add_component(PhysicsComponent(physAttribs))
    local sprite = ball:add_component(Sprite("ball", 32, 32, 0, 0, 0))
    sprite:generate_uvs()

    self.ball_count = self.ball_count + 1
    return ball
end

function PhysicsShowcase:init()
    -- Create Test Entities
    self.ball = self:create_ball(320, 64, 2, 2)

    -- Create Follow Camera
    self.camera = FollowCamera(
        FollowCamParams({
            scale = 1,
            max_x = 20000,
            max_y = 2000,
            springback = 2.0
        }),
        self.ball
    )

    -- Walls
    self.walls.bottom = self:create_wall("WallBottom", 10000, self.wall_thickness, 0, 464)
    self.walls.top = self:create_wall("WallTop", self.window_width - self.wall_thickness, self.wall_thickness, 0, 0)
    self.walls.left = self:create_wall("WallLeft", self.wall_thickness, self.window_height, 0, 0)
    self.walls.right = self:create_wall("WallRight", self.wall_thickness, self.window_height, self.window_width - self.wall_thickness, 0)

    -- Counter UI
    local countEnt = Entity("", "")
    countEnt:add_component(Transform(vec2(10, 48), vec2(1, 1), 0))
    countEnt:add_component(TextComponent("Ball Count: ", "pixel", 4, -1.0, Color(0, 0, 0, 255)))

    local valEnt = Entity("", "")
    valEnt:add_component(Transform(vec2(352, 48), vec2(1, 1), 0))
    self.val_text = valEnt:add_component(TextComponent("0", "pixel", 4, -1.0, Color(0, 0, 0, 255)))
end

function PhysicsShowcase:input()
    if Mouse.just_released(LEFT_BTN) then
        local pos = Mouse:world_position()
        self:create_ball(pos.x, pos.y, 1, 1)
    end
end

function PhysicsShowcase:update_entity(entity)
    entity = entity or self.ball
    if not entity then return end

    local physics = entity:get_component(PhysicsComponent)
    local transform = entity:get_component(Transform)

    local velocity = physics:get_linear_velocity()

    if velocity.y > 0.0 then
        physics:set_gravity_scale(15.0)
    else
        physics:set_gravity_scale(5.0)
    end

    if Keyboard.pressed(KEY_D) then
        physics:set_linear_velocity(vec2(25, velocity.y))
    elseif Keyboard.pressed(KEY_A) then
        physics:set_linear_velocity(vec2(-25, velocity.y))
    end

    if Keyboard.pressed(KEY_W) then
        physics:set_linear_velocity(vec2(velocity.x, velocity.y))
        physics:linear_impulse(vec2(0, -3000))
    end
end

function PhysicsShowcase:update()
    self:update_entity(self.ball)
    self:input()
    if self.camera then
        self.camera:update()
    end
    if self.val_text then
        self.val_text.textStr = tostring(self.ball_count)
    end
end

function PhysicsShowcase:render()
    -- Debug drawings or showcase rendering logic
    -- DrawLine(Line(vec2(50, 50), vec2(200, 200), Color(255, 0, 0, 255)))
    -- DrawLine(Line(vec2(200, 50), vec2(50, 200), Color(0, 0, 255, 255)))
    -- DrawFilledRect(Rect(vec2(300, 300), 100, 100, Color(255, 0, 0, 255)))
    -- DrawText(Text(vec2(164.0, 156.0), "Otterus2D.", "pixel", 320.0, Color(255, 255, 0, 255)))
    -- DrawCircle(Circle(vec2(100, 100), 1.0, 32 , Color(255, 0, 0, 255)))
end

-- Aliases and helper bindings
BallShowcase = PhysicsShowcase

function createBall(pos_x, pos_y, scale_x, scale_y)
    return PhysicsShowcase:create_ball(pos_x, pos_y, scale_x, scale_y)
end

function createWall(name, width, height, x, y)
    return PhysicsShowcase:create_wall(name, width, height, x, y)
end
