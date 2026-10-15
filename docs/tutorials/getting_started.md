# Quickstart: Building a 2D Scene

This guide walks through creating a playable 2D scene from scratch using Lua and Otterus2D. By the end, you will have a sprite on screen responding to keyboard movement and playing a sound effect.

---

## Prerequisites

1. Otterus2D built using CMake and Visual Studio 2022.
2. The runtime dependencies bundle extracted to `./Dependencies`.
3. The editor executable available at `build/bin/Release/OTTERUS_EDITOR.exe` (or `build/bin/OTTERUS_EDITOR.exe`).

---

## 1. Directory Structure

Place your script and assets inside the `assets/` directory adjacent to the executable:

```text
assets/
├── scripts/
│   └── main.lua
├── textures/
│   └── player.png
└── sounds/
    └── jump.wav
```

---

## 2. Setting Up the Main Script

Open `assets/scripts/main.lua`. Every Otterus2D project requires a `main` table exporting `update` and `render` functions.

```lua
-- Global game state
local player = nil
local speed = 200.0

main = {
    [1] = {
        update = function(entity)
            -- 1. Initialize on the first frame
            if not player then
                InitializeGame()
            end

            -- 2. Process player input
            UpdatePlayer()
        end
    },
    [2] = {
        render = function(entity)
            -- Custom rendering calls (optional)
        end
    }
}
```

---

## 3. Loading Assets and Spawning the Entity

Implement `InitializeGame()` to cache your textures and audio, then instantiate the player entity:

```lua
function InitializeGame()
    -- Load texture and sound into the central AssetManager
    AssetManager.add_texture("player_tex", "assets/textures/player.png", true)
    AssetManager.add_sound("sfx_jump", "assets/sounds/jump.wav")

    -- Instantiate entity with tag and group
    player = Entity("Player", "Actors")

    -- Attach spatial transform (Position: x=100, y=100; Scale: 1x; Rotation: 0)
    player:add_component(
        Transform(
            vec2(100.0, 100.0),
            vec2(1.0, 1.0),
            0.0
        )
    )

    -- Attach sprite (Texture: "player_tex", Width: 32, Height: 32, Layer: 1)
    local sprite = player:add_component(
        Sprite(
            "player_tex",
            32.0,
            32.0,
            0,
            0,
            1
        )
    )
    sprite:generate_uvs()

    OTLog.log("Player initialized at (100, 100)")
end
```

---

## 4. Handling Movement and Audio

Implement `UpdatePlayer()` to query the `Keyboard` system each frame:

```lua
function UpdatePlayer()
    if not player then return end

    local transform = player:get_component(Transform)
    if not transform then return end

    -- Continuous 4-directional movement
    if Keyboard.pressed(KEY_W) or Keyboard.pressed(KEY_UP) then
        transform.position.y = transform.position.y - 2.0
    end
    if Keyboard.pressed(KEY_S) or Keyboard.pressed(KEY_DOWN) then
        transform.position.y = transform.position.y + 2.0
    end
    if Keyboard.pressed(KEY_A) or Keyboard.pressed(KEY_LEFT) then
        transform.position.x = transform.position.x - 2.0
    end
    if Keyboard.pressed(KEY_D) or Keyboard.pressed(KEY_RIGHT) then
        transform.position.x = transform.position.x + 2.0
    end

    -- Trigger a sound effect on Space press
    if Keyboard.just_pressed(KEY_SPACE) then
        Sound.play("sfx_jump")
        OTLog.log("Jump sound triggered")
    end
end
```

---

## 5. Running the Scene

1. Launch `OTTERUS_EDITOR.exe`.
2. In the **Scene Viewport**, click the **Play** button.
3. The editor loads `assets/scripts/main.lua`, compiles the script via Sol3, and executes `update()` each tick.
4. Use `W`, `A`, `S`, `D` to move your sprite, and press `Space` to play the sound.
5. Check the **Log Display** panel to verify diagnostic output from `OTLog.log`.
6. Click **Stop** to unload the scene and reset the ECS registry.
