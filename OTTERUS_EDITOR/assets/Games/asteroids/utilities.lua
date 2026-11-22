SCREEN_WIDTH = 640
SCREEN_HEIGHT = 480
KEY_RETURN = KEY_ENTER

function CheckPosition(position, width, height)
    local w = width or 0
    local h = height or 0
    local margin = math.max(w, h) * 0.5

    if position.x < -margin then
        position.x = SCREEN_WIDTH + margin - 2
    elseif position.x > SCREEN_WIDTH + margin then
        position.x = -margin + 2
    end

    if position.y < -margin then
        position.y = SCREEN_HEIGHT + margin - 2
    elseif position.y > SCREEN_HEIGHT + margin then
        position.y = -margin + 2
    end
end

function GetRandomPosition(safe_x, safe_y, min_safe_dist)
    safe_x = safe_x or (SCREEN_WIDTH * 0.5)
    safe_y = safe_y or (SCREEN_HEIGHT * 0.5)
    min_safe_dist = min_safe_dist or 130

    for _ = 1, 30 do
        local x = math.random(30, SCREEN_WIDTH - 30)
        local y = math.random(30, SCREEN_HEIGHT - 30)
        local dx = x - safe_x
        local dy = y - safe_y
        if (dx * dx + dy * dy) >= (min_safe_dist * min_safe_dist) then
            return vec2(x, y)
        end
    end
    return vec2(math.random(30, 120), math.random(30, 120))
end

function GetRandomVelocity(min_speed, max_speed)
    local vx = min_speed + math.random() * (max_speed - min_speed)
    local vy = min_speed + math.random() * (max_speed - min_speed)
    if math.abs(vx) < 0.4 then vx = (vx < 0 and -0.75 or 0.75) end
    if math.abs(vy) < 0.4 then vy = (vy < 0 and -0.75 or 0.75) end
    return vec2(vx, vy)
end

local bg_entities = {}

function LoadBackground()
    ClearBackground()
    for i = 0, 2 do
        for j = 0, 3 do
            local bgtile = Entity("", "bg")
            bgtile:add_component(Transform(vec2(j * 256, i * 256), vec2(1, 1), 0))
            local sprite = bgtile:add_component(Sprite("bg", 256.0, 256.0, 0, 0, 0))
            sprite:generate_uvs()
            table.insert(bg_entities, bgtile)
        end
    end
end

function ClearBackground()
    for _, ent in ipairs(bg_entities) do
        ent:kill()
    end
    bg_entities = {}
end

function LoadEntityDef(def)
    assert(def, "Def does not exist.")
    local tag = def.tag or ""
    local group = def.group or ""
    local ent = Entity(tag, group)

    if def.components.transform then
        local t = def.components.transform
        ent:add_component(
            Transform(
                vec2(t.position.x, t.position.y),
                vec2(t.scale.x, t.scale.y),
                t.rotation or 0
            )
        )
    end

    if def.components.sprite then
        local s = def.components.sprite
        local sprite = ent:add_component(
            Sprite(
                s.asset_name,
                s.width,
                s.height,
                s.start_x or 0,
                s.start_y or 0,
                s.layer or 0
            )
        )
        sprite:generate_uvs()
    end

    if def.components.circle_collider then
        local cc = def.components.circle_collider
        ent:add_component(CircleCollider(cc.radius, cc.offset or vec2(0, 0)))
    end

    return ent:id()
end
