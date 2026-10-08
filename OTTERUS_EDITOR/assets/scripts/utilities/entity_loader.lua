-- Entity Loader Utility
-- Creates and configures entities from definition tables

EntityLoader = {}

function EntityLoader.Create(def)
    assert(def, "Entity definition must not be nil.")

    local tag   = def.tag or ""
    local group = def.group or ""
    local entity = Entity(tag, group)

    local comps = def.components or {}

    -- Transform component
    if comps.transform then
        local t = comps.transform
        entity:add_component(
            Transform(
                vec2(t.position.x, t.position.y),
                vec2(t.scale.x, t.scale.y),
                t.rotation or 0
            )
        )
    end

    -- Sprite component
    if comps.sprite then
        local s = comps.sprite
        local sprite = entity:add_component(
            Sprite(
                s.asset_name,
                s.width,
                s.height,
                s.start_x or 0,
                s.start_y or 0,
                s.layer or 0
            )
        )
        if s.color then
            sprite.color = s.color
        end
        sprite:generate_uvs()
    end

    -- Animation component
    if comps.animation then
        local a = comps.animation
        entity:add_component(
            Animation(
                a.numFrames,
                a.frameRate,
                a.frameOffset or 0,
                a.isVertical or false,
                a.looped ~= false
            )
        )
    end

    -- Box or Circle Collider
    if comps.box_collider then
        local bc = comps.box_collider
        local offset = bc.offset and vec2(bc.offset.x, bc.offset.y) or vec2(0, 0)
        entity:add_component(BoxCollider(bc.width, bc.height, offset))
    elseif comps.circle_collider then
        local cc = comps.circle_collider
        local offset = cc.offset and vec2(cc.offset.x, cc.offset.y) or vec2(0, 0)
        entity:add_component(CircleCollider(cc.radius, offset))
    end

    -- Physics component
    if comps.physics then
        local p = comps.physics
        local attrs = PhysicsAttributes()
        attrs.type          = p.type or BodyType.STATIC
        attrs.density       = p.density or 1.0
        attrs.friction      = p.friction or 0.2
        attrs.restitution   = p.restitution or 0.0
        attrs.gravityScale  = p.gravityScale or 0.0
        attrs.fixedRotation = (p.fixedRotation ~= nil) and p.fixedRotation or true

        local t = comps.transform
        if t then
            attrs.position = vec2(t.position.x, t.position.y)
            attrs.scale    = vec2(t.scale.x, t.scale.y)
        end

        if comps.box_collider then
            attrs.boxShape = true
            attrs.boxSize  = vec2(comps.box_collider.width, comps.box_collider.height)
            if comps.box_collider.offset then
                attrs.offset = vec2(comps.box_collider.offset.x, comps.box_collider.offset.y)
            end
        elseif comps.circle_collider then
            attrs.circle = true
            attrs.radius = comps.circle_collider.radius
            if comps.circle_collider.offset then
                attrs.offset = vec2(comps.circle_collider.offset.x, comps.circle_collider.offset.y)
            end
        end

        attrs.isSensor = p.isSensor or false
        attrs.objectData = ObjectData(tag, group, not attrs.isSensor, attrs.isSensor, entity:id())
        entity:add_component(PhysicsComponent(attrs))
    end

    return entity:id()
end

function EntityLoader.SpawnAnimal(def, x, y)
    if not def then return nil end

    -- Guard against missing textures: if the animal sprite texture isn't loaded, skip spawning
    if def.components and def.components.sprite then
        local texName = def.components.sprite.asset_name
        if AssetLoader and not AssetLoader.HasTexture(texName) then
            return nil
        end
    end

    local customDef = {
        tag   = def.tag,
        group = def.group,
        components = {
            transform = {
                position = { x = x or def.components.transform.position.x, y = y or def.components.transform.position.y },
                scale    = { x = def.components.transform.scale.x, y = def.components.transform.scale.y },
                rotation = def.components.transform.rotation or 0
            },
            sprite          = def.components.sprite,
            animation       = def.components.animation,
            circle_collider = def.components.circle_collider,
            box_collider    = def.components.box_collider,
            physics         = def.components.physics
        }
    }
    return EntityLoader.Create(customDef)
end

function EntityLoader.CreateText(params)
    params = params or {}
    local entity = Entity(params.tag or "", params.group or "UI")
    local pos = params.position or vec2(0, 0)
    local scale = params.scale or vec2(1, 1)
    entity:add_component(Transform(pos, scale, 0))

    local textComp = entity:add_component(
        TextComponent(
            params.text or "",
            params.font or "pixel",
            params.padding or 0,
            params.wrap or -1.0,
            params.color or Color(255, 255, 255, 255)
        )
    )
    if params.hidden ~= nil then
        textComp.isHidden = params.hidden
    end
    return entity, textComp
end

-- Backward compatibility aliases
function LoadEntity(def)
    return EntityLoader.Create(def)
end

function SpawnAnimal(def, x, y)
    return EntityLoader.SpawnAnimal(def, x, y)
end
