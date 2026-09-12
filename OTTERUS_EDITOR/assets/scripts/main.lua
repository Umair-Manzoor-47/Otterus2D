math.randomseed(os.time())

run_script("assets/scripts/asset_defs.lua")
run_script("assets/scripts/tilemap/test_map.lua")
run_script("assets/scripts/utilities.lua")

-- Window bounds
local WINDOW_WIDTH = 640
local WINDOW_HEIGHT = 480
local WALL_THICKNESS = 16

function createWall(name, width, height, x, y)
    local wall = Entity(name, "")
    local collider = wall:add_component(BoxCollider(width, height, vec2(0, 0)))
    local transform = wall:add_component(Transform(vec2(x, y), vec2(1, 1), 0))

    local physAttrs = PhysicsAttributes()
    physAttrs.type = BodyType.STATIC
    physAttrs.density = 1000.0
    physAttrs.friction = 0.0
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

function createBall(pos_x, pos_y, scale_x, scale_y)
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
    return ball
end



--AssetManager.add_font("Minecraft", "assets/fonts/Minecraft.ttf", 32.0)


--local tilemap = CreateTestMap()
--assert(tilemap)
LoadAssets(AssetDefs)
--LoadMap(tilemap)
--Music.play("bgm")
--LoadEntity(AnimalsDefs.baby_bunny)
--LoadEntity(AnimalsDefs.baby_bunny_grey)

-- Create Test Entities
ball = createBall(320, 64, 2, 2)

-- Walls
local wallBottom = createWall("WallBottom", WINDOW_WIDTH - WALL_THICKNESS, WALL_THICKNESS, 0, 464)
local wallTop = createWall("WallTop", WINDOW_WIDTH - WALL_THICKNESS, WALL_THICKNESS, 0, 0)
local wallLeft = createWall("WallLeft", WALL_THICKNESS, WINDOW_HEIGHT, 0, 0)
local wallRight = createWall("WallRight", WALL_THICKNESS, WINDOW_HEIGHT, WINDOW_WIDTH - WALL_THICKNESS, 0)

main = {
    [1] = {
        update = function()
            update_entity( ball )
            input()
            
        end
    },

    [2] = {
        render = function()
            --DrawLine(Line(vec2(50, 50), vec2(200, 200), Color(255, 0, 0, 255)))
            --DrawLine(Line(vec2(200, 50), vec2(50, 200), Color(0, 0, 255, 255)))
            --DrawFilledRect(Rect(vec2(300, 300), 100, 100, Color(255, 0, 0, 255)))
            --DrawText(Text(vec2(164.0, 156.0), "Otterus2D.", "Minecraft", 320.0, Color(255, 255, 0, 255)))
            --DrawCircle(Circle(vec2(100, 100), 1.0, 32 , Color(255, 0, 0, 255)))
        end
    }
}



function input()
	if Mouse.just_released(LEFT_BTN) then
        local pos_x, pos_y = Mouse:screen_position()
        createBall(pos_x, pos_y, 1, 1)
    end
end

function update_entity( entity )
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

    if Keyboard.pressed(KEY_S) then
        physics:set_linear_velocity(vec2(25, velocity.y))
        physics:linear_impulse(vec2(0, -3000))
    end

end
