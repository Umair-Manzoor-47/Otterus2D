-- Asset Loader Utility
-- Manages asset loading and provides safe accessors to prevent crashes on missing files

AssetLoader = {
    Loaded = {
        textures = {},
        music    = {},
        sfx      = {},
        fonts    = {}
    }
}

local function resolve_asset_path(path)
    if not path or path == "" then return path end
    local f = io.open(path, "r")
    if f ~= nil then
        f:close()
        return path
    end
    local candidates = {
        "../" .. path,
        "OTTERUS_EDITOR/" .. path,
        path:gsub("^%.%./Games/", "assets/Games/"),
        path:gsub("^Games/", "assets/Games/"),
        path:gsub("^assets/textures/Platformer/", "assets/Games/Platformer/textures/"),
        path:gsub("^assets/textures/CozyScene/", "assets/Games/CozyScene/textures/"),
        path:gsub("^assets/textures/asteroids/", "assets/Games/asteroids/textures/"),
        "assets/Games/" .. path,
        "../assets/Games/" .. path,
        "assets/" .. path
    }
    for _, cand in ipairs(candidates) do
        local fc = io.open(cand, "r")
        if fc ~= nil then
            fc:close()
            return cand
        end
    end
    return path
end

function AssetLoader.Load(assets)
    if not assets then return end

    if assets.textures then
        for _, v in pairs(assets.textures) do
            local tileset = v.tileset or false
            local resolvedPath = resolve_asset_path(v.path)
            if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.TEXTURE) then
                AssetLoader.Loaded.textures[v.name] = true
            elseif AssetManager.add_texture(v.name, resolvedPath, v.pixel_art, tileset) then
                AssetLoader.Loaded.textures[v.name] = true
            else
                AssetLoader.Loaded.textures[v.name] = nil
                OT_warn("Failed to load texture [%s] at path [%s]", v.name, resolvedPath)
            end
        end
    end

    if assets.music then
        for _, v in pairs(assets.music) do
            local resolvedPath = resolve_asset_path(v.path)
            if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.MUSIC) then
                AssetLoader.Loaded.music[v.name] = true
            elseif AssetManager.add_music(v.name, resolvedPath) then
                AssetLoader.Loaded.music[v.name] = true
            else
                AssetLoader.Loaded.music[v.name] = nil
                OT_error("Failed to add music [%s] at path [%s]", v.name, resolvedPath)
            end
        end
    end

    if assets.sfx then
        for _, v in pairs(assets.sfx) do
            local resolvedPath = resolve_asset_path(v.path)
            if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.SOUNDFX) then
                AssetLoader.Loaded.sfx[v.name] = true
            elseif AssetManager.add_sound(v.name, resolvedPath) then
                AssetLoader.Loaded.sfx[v.name] = true
            else
                AssetLoader.Loaded.sfx[v.name] = nil
                OT_error("Failed to add sfx [%s] at path [%s]", v.name, resolvedPath)
            end
        end
    end

    if assets.fonts then
        for _, v in pairs(assets.fonts) do
            local resolvedPath = resolve_asset_path(v.path)
            if AssetManager.has_asset and AssetManager.has_asset(v.name, AssetType.FONT) then
                AssetLoader.Loaded.fonts[v.name] = true
            elseif AssetManager.add_font(v.name, resolvedPath, v.size) then
                AssetLoader.Loaded.fonts[v.name] = true
            else
                AssetLoader.Loaded.fonts[v.name] = nil
                OT_error("Failed to add font [%s] at path [%s]", v.name, resolvedPath)
            end
        end
    end
end

function AssetLoader.HasTexture(name)
    if AssetLoader.Loaded.textures[name] == true then
        return true
    end
    if AssetManager and AssetManager.has_asset and AssetManager.has_asset(name, AssetType.TEXTURE) then
        AssetLoader.Loaded.textures[name] = true
        return true
    end
    return false
end

function AssetLoader.HasMusic(name)
    if AssetLoader.Loaded.music[name] == true then
        return true
    end
    if AssetManager and AssetManager.has_asset and AssetManager.has_asset(name, AssetType.MUSIC) then
        AssetLoader.Loaded.music[name] = true
        return true
    end
    return false
end

function AssetLoader.HasSound(name)
    if AssetLoader.Loaded.sfx[name] == true then
        return true
    end
    if AssetManager and AssetManager.has_asset and AssetManager.has_asset(name, AssetType.SOUNDFX) then
        AssetLoader.Loaded.sfx[name] = true
        return true
    end
    return false
end

function AssetLoader.HasFont(name)
    if AssetLoader.Loaded.fonts[name] == true then
        return true
    end
    if AssetManager and AssetManager.has_asset and AssetManager.has_asset(name, AssetType.FONT) then
        AssetLoader.Loaded.fonts[name] = true
        return true
    end
    return false
end

function AssetLoader.PlayMusic(name, loops)
    if not AssetLoader.HasMusic(name) then return end
    if loops then
        Music.play(name, loops)
    else
        Music.play(name)
    end
end

function AssetLoader.PlaySound(name, loops, channel)
    if not AssetLoader.HasSound(name) then return end
    if loops and channel then
        Sound.play(name, loops, channel)
    else
        Sound.play(name)
    end
end

-- Backward compatibility alias
function LoadAssets(assets)
    return AssetLoader.Load(assets)
end
