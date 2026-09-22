TileLayerLoader = {}
ColliderLayerLoader = {}
TriggerLayerLoader = {}
PlatformerMapLoader = {}

function TileLayerLoader:Load(layerDef, tilesets, scale, layerIndex)
    local rows = layerDef.height - 1
    local cols = layerDef.width
    local layer = layerIndex or 0
    local entities = {}

    for row = 0, rows do
        for col = 1, cols do
            local id = layerDef.data[row * cols + col]
            if id and id ~= 0 then
                local tileset = GetTileset(tilesets, id)
                if tileset then
                    local start_x, start_y = tileset:GetTileStartXY(id)
                    local tileW = tileset.tilewidth * scale
                    local tileH = tileset.tileheight * scale
                    local position = vec2((col - 1) * tileW, row * tileH)

                    local tile = Entity("", "tiles")
                    tile:add_component(Transform(position, vec2(scale, scale), 0))
                    local sprite = tile:add_component(
                        Sprite(
                            tileset.name,
                            tileset.tilewidth,
                            tileset.tileheight,
                            start_x,
                            start_y,
                            layer
                        )
                    )
                    sprite:generate_uvs()
                    table.insert(entities, tile)
                end
            end
        end
    end

    return entities
end

local function CreatePhysicsBoxes(layerDef, scale, isSensor, tag, group)
    local rows = layerDef.height - 1
    local cols = layerDef.width
    local tileDim = 16 * scale
    local entities = {}

    for row = 0, rows do
        local col = 1
        while col <= cols do
            local id = layerDef.data[row * cols + col]
            if id and id ~= 0 then
                local startCol = col
                while col <= cols and layerDef.data[row * cols + col] ~= 0 do
                    col = col + 1
                end

                local span = col - startCol
                local boxW = span * tileDim
                local boxH = tileDim
                local posX = (startCol - 1) * tileDim
                local posY = row * tileDim

                local ent = Entity(tag, group)
                local collider = ent:add_component(BoxCollider(boxW, boxH, vec2(0, 0)))
                local transform = ent:add_component(Transform(vec2(posX, posY), vec2(1, 1), 0))

                local attrs = PhysicsAttributes()
                attrs.type          = BodyType.STATIC
                attrs.density       = isSensor and 0.0 or 1000.0
                attrs.friction      = isSensor and 0.0 or 0.5
                attrs.restitution   = 0.0
                attrs.gravityScale  = isSensor and 0.0 or 1.0
                attrs.position      = transform.position
                attrs.scale         = transform.scale
                attrs.boxShape      = true
                attrs.boxSize       = vec2(collider.width, collider.height)
                attrs.fixedRotation = not isSensor and false or true
                attrs.isSensor      = isSensor

                ent:add_component(PhysicsComponent(attrs))
                table.insert(entities, ent)
            else
                col = col + 1
            end
        end
    end

    return entities
end

function ColliderLayerLoader:Load(layerDef, scale)
    return CreatePhysicsBoxes(layerDef, scale, false, "PlatformCollider", "ground")
end

function TriggerLayerLoader:Load(layerDef, scale)
    return CreatePhysicsBoxes(layerDef, scale, true, "PlatformTrigger", "trigger")
end

function PlatformerMapLoader:Load(mapDef, config)
    assert(mapDef, "Map definition must be provided")
    config = config or {}

    local scale        = config.scale or 2
    local loadVisuals  = config.visuals ~= false
    local loadColls    = config.colliders ~= false
    local loadTriggers = config.triggers ~= false

    local tilesets = {}
    for _, v in ipairs(mapDef.tilesets or {}) do
        local ts = Tileset:Create({
            name       = v.name,
            columns    = v.columns or 22,
            width      = v.imagewidth or 352,
            height     = v.imageheight or 176,
            tilewidth  = v.tilewidth or mapDef.tilewidth or 16,
            tileheight = v.tileheight or mapDef.tileheight or 16,
            firstgid   = v.firstgid or 1
        })
        table.insert(tilesets, ts)
    end

    local result = {
        width        = mapDef.width,
        height       = mapDef.height,
        tile_size    = (mapDef.tilewidth or 16) * scale,
        pixel_width  = mapDef.width * (mapDef.tilewidth or 16) * scale,
        pixel_height = mapDef.height * (mapDef.tileheight or 16) * scale,
        visuals      = {},
        colliders    = {},
        triggers     = {}
    }

    for idx, layer in ipairs(mapDef.layers or {}) do
        local nameLower = string.lower(layer.name or "")

        if nameLower == "colliders" then
            if loadColls then
                result.colliders = ColliderLayerLoader:Load(layer, scale)
            end
        elseif nameLower == "triggers" then
            if loadTriggers then
                result.triggers = TriggerLayerLoader:Load(layer, scale)
            end
        else
            if loadVisuals then
                local tiles = TileLayerLoader:Load(layer, tilesets, scale, idx - 1)
                for _, t in ipairs(tiles) do
                    table.insert(result.visuals, t)
                end
            end
        end
    end

    return result
end
