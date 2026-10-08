-- Player Entity Factory
-- Handles instantiation of the player character and UI entities

PlayerFactory = {}

function PlayerFactory.CreatePlayer(x, y)
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
    attrs.objectData    = ObjectData("NinjaFrog", "player", true, false, player:id())

    local physics = player:add_component(PhysicsComponent(attrs))
    local sprite  = player:add_component(Sprite("nf_idle", 32, 32, 0, 0, 1))
    sprite:generate_uvs()

    local anim = player:add_component(Animation(11, 10, 0, false, true))

    return player, physics, sprite, anim
end

function PlayerFactory.CreateUI()
    -- UI camera is 640x480. Position death count at top right (X=480, Y=40)
    local ui_entity = Entity("DeathCount_UI", "UI")
    ui_entity:add_component(Transform(vec2(480, 40), vec2(1, 1), 0))
    local text_comp = ui_entity:add_component(TextComponent("Deaths: 0", "pixel", 0, -1.0, Color(0, 0, 0, 255)))
    text_comp.isHidden = true
    return ui_entity, text_comp
end
