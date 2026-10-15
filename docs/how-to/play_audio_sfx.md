# How to Play Audio and Music

Otterus2D provides dual-pipeline audio playback powered by SDL2_mixer, featuring 44.1 kHz stereo output, one background music stream, and 16 hardware mixing channels for concurrent sound effects.

---

## 1. Loading Audio Assets

Preload audio tracks during scene initialization using the `AssetManager`:

```lua
function LoadAudio()
    -- Streaming music (MP3 / OGG)
    AssetManager.add_music("bgm_level1", "assets/sounds/music.mp3")

    -- Short sound effects (WAV)
    AssetManager.add_sound("sfx_laser", "assets/sounds/laser.wav")
    AssetManager.add_sound("sfx_explosion", "assets/sounds/explosion.wav")
end
```

---

## 2. Background Music Playback (`Music`)

Only one music track can stream at a time. The engine automatically handles background decoding.

### Playback Controls

```lua
-- Play and loop indefinitely (-1)
Music.play("bgm_level1", -1)

-- Play a specific number of times (e.g., once)
Music.play("bgm_level1", 1)

-- Pause and resume
Music.pause()
Music.resume()

-- Adjust volume (Scale: 0 to 100)
Music.set_volume(80)

-- Check playback state
if Music.is_playing() then
    -- Music is actively streaming
end

-- Halt playback
Music.stop()
```

---

## 3. Sound Effects Playback (`Sound`)

Sound effects play over 16 allocated mixer channels (`0` through `15`). Multiple sound effects can play simultaneously.

### Triggering Sound Effects

```lua
-- Play on the first available channel (channel = -1)
Sound.play("sfx_laser")

-- Play on a designated channel with loop count
-- Sound.play(name, loops, channel)
Sound.play("sfx_explosion", 0, -1)
```

### Channel Management and Volume

```lua
-- Set volume on a specific channel (channel, volume: 0 to 100)
Sound.set_volume(0, 50)

-- Check if a specific channel is busy
if Sound.is_playing(0) then
    -- Channel 0 is busy
end

-- Stop sound on a specific channel
Sound.stop(0)

-- Stop ALL audio channels (-1)
Sound.stop(-1)
```

---

## 4. Best Practice: Central Audio Definition Table

Organize audio assets into a structured table for declarative preloading:

```lua
AudioDefs = {
    music = {
        { name = "title_theme", path = "assets/sounds/title.mp3" },
        { name = "level_theme", path = "assets/sounds/level.mp3" }
    },
    sfx = {
        { name = "jump",  path = "assets/sounds/jump.wav" },
        { name = "laser", path = "assets/sounds/laser.wav" },
        { name = "hit",   path = "assets/sounds/hit.wav" }
    }
}

function PreloadAllAudio(defs)
    for _, item in ipairs(defs.music) do
        AssetManager.add_music(item.name, item.path)
    end
    for _, item in ipairs(defs.sfx) do
        AssetManager.add_sound(item.name, item.path)
    end
end
```
