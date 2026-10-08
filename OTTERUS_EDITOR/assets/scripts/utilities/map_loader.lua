-- Map Loader Utility
-- Unified tilemap and collision/trigger loader for 2D orthogonal Tiled maps

Tileset = {}
Tileset.__index = Tileset

function Tileset:Create(params)
    local this = {
        name       = params.name,
        columns    = params.columns,
        width      = params.width,
        height     = params.height,
        tilewidth  = params.tilewidth,
        tileheight = params.tileheight,
        firstgid   = params.firstgid
    }
    this.rows    = params.height / params.tileheight
    this.lastgid = math.floor(((this.rows * this.columns) + this.firstgid) - 1)
    setmetatable(this, self)
    return this
end

function Tileset:TileIdExists(id)
    return id >= self.firstgid and id <= self.lastgid
end

function Tileset:GetTileStartXY(id)
    assert(self:TileIdExists(id), "Tile [" .. tostring(id) .. "] does not exist in Tileset [" .. tostring(self.name) .. "]")
    local actualTileId = id - self.firstgid
    local start_y = math.floor(actualTileId / self.columns)
    local start_x = math.floor(actualTileId % self.columns)
    return start_x, start_y
end

function GetTileset(tilesets, id)
    for _, ts in pairs(tilesets) do
        if ts:TileIdExists(id) then
            return ts
        end
    end
    return nil
end

TileLayerLoader = {}

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

ColliderLayerLoader = {}

function ColliderLayerLoader:Load(layerDef, scale, isSensor, customTag, customGroup)
    local rows = layerDef.height - 1
    local cols = layerDef.width
    local tileDim = 16 * scale
    local tag   = customTag or (isSensor and "PlatformTrigger" or "PlatformCollider")
    local group = customGroup or (isSensor and "trigger" or "ground")
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
                attrs.isSensor      = isSensor or false
                attrs.objectData    = ObjectData(tag, group, not attrs.isSensor, attrs.isSensor, ent:id())
                ent:add_component(PhysicsComponent(attrs))
                table.insert(entities, ent)
            else
                col = col + 1
            end
        end
    end

    return entities
end

TriggerLayerLoader = {}

function TriggerLayerLoader:Load(layerDef, scale)
    return ColliderLayerLoader:Load(layerDef, scale, true, "PlatformTrigger", "trigger")
end

MapLoader = {}

function MapLoader:Load(mapDef, config)
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
        triggers     = {},
        tilesets     = tilesets
    }

    for idx, layer in ipairs(mapDef.layers or {}) do
        local nameLower = string.lower(layer.name or "")
        local isColliderLayer = (nameLower == "colliders")
            or (layer.properties and (layer.properties.colliders or layer.properties.has_colliders or layer.properties.collider))
            or string.find(nameLower, "overgrown") ~= nil
            or string.find(nameLower, "tree") ~= nil

        local isTriggerLayer = (nameLower == "triggers")
            or (layer.properties and (layer.properties.triggers or layer.properties.is_trigger))

        if isTriggerLayer then
            if loadTriggers then
                local trigs = TriggerLayerLoader:Load(layer, scale)
                for _, ent in ipairs(trigs) do
                    table.insert(result.triggers, ent)
                end
            end
        elseif isColliderLayer then
            -- If it is a collider layer in platformer, it only generates physics colliders
            -- If it has visuals in cosy scene (e.g. overgrown or tree), load both visuals and colliders
            if loadVisuals and nameLower ~= "colliders" then
                local tiles = TileLayerLoader:Load(layer, tilesets, scale, idx - 1)
                for _, t in ipairs(tiles) do
                    table.insert(result.visuals, t)
                end
            end
            if loadColls then
                local cols = ColliderLayerLoader:Load(layer, scale, false, layer.name or "obstacle", "obstacle")
                for _, ent in ipairs(cols) do
                    table.insert(result.colliders, ent)
                end
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

    function result:destroy()
        for _, ent in ipairs(self.visuals) do
            if ent and ent.kill then ent:kill() end
        end
        for _, ent in ipairs(self.colliders) do
            if ent and ent.kill then ent:kill() end
        end
        for _, ent in ipairs(self.triggers) do
            if ent and ent.kill then ent:kill() end
        end
        self.visuals = {}
        self.colliders = {}
        self.triggers = {}
    end

    return result
end

-- Backward compatibility aliases
PlatformerMapLoader = MapLoader

function LoadMap(mapDef, config)
    return MapLoader:Load(mapDef, config)
end
