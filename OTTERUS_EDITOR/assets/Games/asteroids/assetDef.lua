local function resolve_path(path)
    if io.open(path, "r") then
        return path
    end
    local candidates = {
        "assets/Games/asteroids/" .. path,
        "../assets/Games/asteroids/" .. path,
        "OTTERUS_EDITOR/assets/Games/asteroids/" .. path,
        "../" .. path
    }
    for _, cand in ipairs(candidates) do
        local f = io.open(cand, "r")
        if f then
            f:close()
            return cand
        end
    end
    return path
end

AssetDefs = AssetDefs or {}
AsteroidsAssetDefs = {
    textures = {
        { name = "ship", path = resolve_path("textures/ship.png"), pixel_art = true, tileset = false },
        { name = "ast1", path = resolve_path("textures/meteor_small.png"), pixel_art = true, tileset = false },
        { name = "ast2", path = resolve_path("textures/meteor_big.png"), pixel_art = true, tileset = false },
        { name = "bg", path = resolve_path("textures/bg.png"), pixel_art = true, tileset = false },
        { name = "laser", path = resolve_path("textures/laser.png"), pixel_art = true, tileset = false },
    },
    music = {
        { name = "bgm", path = resolve_path("music/bgm.mp3") }
    },
    sfx = {
        { name = "laser", path = resolve_path("music/laser.mp3") }
    }
}

function LoadAsteroidsAssets()
    for _, v in pairs(AsteroidsAssetDefs.textures) do
        local tileset = v.tileset or false
        if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.TEXTURE) then
        else
            AssetManager.add_texture(v.name, v.path, v.pixel_art, tileset)
        end
    end

    for _, v in pairs(AsteroidsAssetDefs.music) do
        if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.MUSIC) then
        else
            AssetManager.add_music(v.name, v.path)
        end
    end

    for _, v in pairs(AsteroidsAssetDefs.sfx) do
        if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.SOUNDFX) then
        else
            AssetManager.add_sound(v.name, v.path)
        end
    end
end
