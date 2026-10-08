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

function AssetLoader.Load(assets)
    if not assets then return end

    if assets.textures then
        for _, v in pairs(assets.textures) do
            if AssetManager.add_texture(v.name, v.path, v.pixel_art) then
                AssetLoader.Loaded.textures[v.name] = true
                OT_log("Loaded texture [%s]", v.name)
            else
                AssetLoader.Loaded.textures[v.name] = nil
                OT_error("Failed to load texture [%s] at path [%s]", v.name, v.path)
            end
        end
    end

    if assets.music then
        for _, v in pairs(assets.music) do
            if AssetManager.add_music(v.name, v.path) then
                AssetLoader.Loaded.music[v.name] = true
                OT_log("Added music [%s]", v.name)
            else
                AssetLoader.Loaded.music[v.name] = nil
                OT_error("Failed to add music [%s] at path [%s]", v.name, v.path)
            end
        end
    end

    if assets.sfx then
        for _, v in pairs(assets.sfx) do
            if AssetManager.add_sound(v.name, v.path) then
                AssetLoader.Loaded.sfx[v.name] = true
                OT_log("Added sfx [%s]", v.name)
            else
                AssetLoader.Loaded.sfx[v.name] = nil
                OT_error("Failed to add sfx [%s] at path [%s]", v.name, v.path)
            end
        end
    end

    if assets.fonts then
        for _, v in pairs(assets.fonts) do
            if AssetManager.add_font(v.name, v.path, v.size) then
                AssetLoader.Loaded.fonts[v.name] = true
                OT_log("Added font [%s]", v.name)
            else
                AssetLoader.Loaded.fonts[v.name] = nil
                OT_error("Failed to add font [%s] at path [%s]", v.name, v.path)
            end
        end
    end
end

function AssetLoader.HasTexture(name)
    return AssetLoader.Loaded.textures[name] == true
end

function AssetLoader.HasMusic(name)
    return AssetLoader.Loaded.music[name] == true
end

function AssetLoader.HasSound(name)
    return AssetLoader.Loaded.sfx[name] == true
end

function AssetLoader.HasFont(name)
    return AssetLoader.Loaded.fonts[name] == true
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
