# Lua Scripting API Reference

Otterus2D exposes core engine services, ECS entities, input devices, and resource loaders to Lua 5.3 via Sol3 bindings.

---

## 1. Script Entry Points

Gameplay scripts are loaded from `./assets/scripts/main.lua`. The engine requires a global `main` table containing update and render tables:

```lua
main = {
    [1] = {
        update = function(entity)
            -- Logic update called once per frame
        end
    },
    [2] = {
        render = function(entity)
            -- Render update called once per frame
        end
    }
}
```

---

## 2. ECS Entity and Registry

### Entity (`Entity`)

Entities are instantiated through `Entity(name, group)` or referenced by ID via `Entity(id)`.

| Method | Parameters | Returns | Description |
| :--- | :--- | :--- | :--- |
| `Entity(name, group)` | `string`, `string` | `Entity` | Instantiates a new entity in the registry. |
| `Entity(id)` | `int` | `Entity` | Wraps an existing entity identifier. |
| `entity:add_component(comp)` | `Component` | `Component` | Attaches a component instance to the entity. |
| `entity:get_component(Type)` | `Type` | `Component` | Retrieves attached component or `nil`. |
| `entity:has_component(Type)` | `Type` | `bool` | Checks if component type exists on entity. |
| `entity:remove_component(Type)` | `Type` | None | Detaches component type from entity. |
| `entity:name()` | None | `string` | Returns entity name tag. |
| `entity:group()` | None | `string` | Returns entity group categorization. |
| `entity:id()` | None | `int` | Returns unique integer entity identifier. |
| `entity:kill()` | None | None | Destroys the entity in the registry. |

### Registry Query (`Registry`)

| Method | Parameters | Returns | Description |
| :--- | :--- | :--- | :--- |
| `Registry.get_entities(...)` | Component types | `runtime_view` | Queries entities matching component types. |
| `view:for_each(callback)` | `function(ent)` | None | Iterates over each matching entity. |
| `view:exclude(...)` | Component types | None | Excludes entities matching specified types. |
| `Registry.clear()` | None | None | Purges all entities from the active registry. |

---

## 3. Input System

Input states are queried through static `Keyboard` and `Mouse` user types.

### Keyboard Methods

| Method | Returns | Description |
| :--- | :--- | :--- |
| `Keyboard.pressed(key)` | `bool` | True continuously while key is held down. |
| `Keyboard.just_pressed(key)` | `bool` | True only on the initial frame key is pressed. |
| `Keyboard.just_released(key)` | `bool` | True only on the frame key is released. |

#### Key Constants
- Letters: `KEY_A` through `KEY_Z`
- Numbers: `KEY_0` through `KEY_9`
- Arrows: `KEY_UP`, `KEY_DOWN`, `KEY_LEFT`, `KEY_RIGHT`
- Modifiers: `KEY_LCTRL`, `KEY_RCTRL`, `KEY_LSHIFT`, `KEY_RSHIFT`, `KEY_LALT`, `KEY_RALT`
- Controls: `KEY_SPACE`, `KEY_ENTER`, `KEY_ESC`, `KEY_BACKSPACE`
- Functions: `KEY_F1` through `KEY_F12`

### Mouse Methods

| Method | Returns | Description |
| :--- | :--- | :--- |
| `Mouse.pressed(btn)` | `bool` | True continuously while button is held down. |
| `Mouse.just_pressed(btn)` | `bool` | True only on initial button press. |
| `Mouse.just_released(btn)` | `bool` | True only on button release. |
| `Mouse.screen_position()` | `vec2` | Pixel coordinates in window space. |
| `Mouse.world_position()` | `vec2` | Coordinates transformed into 2D world space. |
| `Mouse.wheel_x()` | `int` | Horizontal scroll wheel delta. |
| `Mouse.wheel_y()` | `int` | Vertical scroll wheel delta. |
| `Mouse.is_moving()` | `bool` | True if mouse moved during current frame. |

#### Mouse Constants
`LEFT_BTN`, `MIDDLE_BTN`, `RIGHT_BTN`

---

## 4. Physics Contacts (`ContactListener`)

Provides Box2D collision pairs each frame.

| Method | Returns | Description |
| :--- | :--- | :--- |
| `ContactListener.get_user_data()` | `ObjectData`, `ObjectData` | Returns pair of colliding objects, or `nil`. |

### Contact Object Fields (`ObjectData`)

- `object.tag`: Tag string.
- `object.group`: Group string.
- `object.entityId`: Integer ID of the entity.
- `object.isTrigger`: True if sensor.
- `object.isCollider`: True if solid obstacle.

---

## 5. Audio Subsystem

Provides music streaming via SDL2_mixer and multi-channel sound effects.

### Music Methods (`Music`)

| Method | Parameters | Description |
| :--- | :--- | :--- |
| `Music.play(name, [loops])` | `string`, `int` | Play background music (`-1` loops indefinitely). |
| `Music.stop()` | None | Stop active music playback. |
| `Music.pause()` | None | Pause playback. |
| `Music.resume()` | None | Resume paused playback. |
| `Music.set_volume(volume)` | `int` (0-128) | Set music volume level. |
| `Music.is_playing()` | None | Returns `bool` indicating active playback. |

### Sound Effects (`Sound`)

| Method | Parameters | Description |
| :--- | :--- | :--- |
| `Sound.play(name, [loops, channel])` | `string`, `int`, `int` | Play cached sound effect on designated channel. |
| `Sound.stop(channel)` | `int` (`-1` for all) | Stop playback on specified channel. |
| `Sound.set_volume(volume, channel)`| `int`, `int` | Adjust channel volume (0-128). |
| `Sound.is_playing(channel)` | `int` | Returns `bool` for channel status. |

---

## 6. AssetManager

Allows dynamic loading and registration of assets into the central cache.

| Method | Parameters | Returns | Description |
| :--- | :--- | :--- | :--- |
| `AssetManager.add_texture(name, path, pixel_art)` | `string`, `string`, `bool` | `bool` | Loads texture file into GPU memory. |
| `AssetManager.add_music(name, path)` | `string`, `string` | `bool` | Loads streaming audio file (MP3/OGG). |
| `AssetManager.add_sound(name, path)` | `string`, `string` | `bool` | Loads sound effect into memory (WAV). |
| `AssetManager.add_font(name, path, size)` | `string`, `string`, `float` | `bool` | Loads TrueType font atlas. |

---

## 7. Camera Control

Controls the 2D orthographic scene camera.

| Method | Parameters | Returns | Description |
| :--- | :--- | :--- | :--- |
| `Camera.position()` | None | `vec2` | Current camera position. |
| `Camera.set_position(pos)` | `vec2` | None | Set world coordinates of camera center. |
| `Camera.scale()` | None | `float` | Current zoom scale. |
| `Camera.set_scale(scale)` | `float` | None | Adjust camera zoom level. |
| `Camera.width()` | None | `int` | Viewport width in pixels. |
| `Camera.height()` | None | `int` | Viewport height in pixels. |
| `Camera.get_world_coords(screenPos)` | `vec2` | `vec2` | Convert screen coordinates to world units. |
| `Camera.get_screen_coords(worldPos)` | `vec2` | `vec2` | Convert world coordinates to screen units. |

---

## 8. Math and Utilities

### 2D Vector (`vec2`)

Vector construction supports `vec2(scalar)` or `vec2(x, y)`. Standard operators (`+`, `-`, `*`, `/`) are overloaded.

```lua
local v1 = vec2(10.0, 20.0)
local v2 = vec2(5.0, 5.0)
local v3 = v1 + v2 * 2.0

local len = v1:length()
local lenSq = v1:lengthSq()
```

### Free Math Functions

| Function | Parameters | Returns | Description |
| :--- | :--- | :--- | :--- |
| `distance(v1, v2)` | `vec2, vec2` | `float` | Euclidean distance between two points. |
| `lerp(a, b, t)` | `float, float, float` | `float` | Linear interpolation from `a` to `b` by factor `t`. |
| `clamp(val, min, max)` | `float, float, float` | `float` | Constrains value between `min` and `max`. |

### Precision Timer (`Timer`)

Used to track cooldowns, frame timing, and delays.

| Method | Returns | Description |
| :--- | :--- | :--- |
| `timer = Timer()` | `Timer` | Constructs a new inactive timer. |
| `timer:start()` | None | Starts tracking elapsed time. |
| `timer:stop()` | None | Stops timer. |
| `timer:restart()` | None | Resets and immediately starts timer. |
| `timer:pause()` | None | Temporarily halts timer. |
| `timer:resume()` | None | Resumes paused timer. |
| `timer:elapsed_ms()` | `int64` | Returns elapsed time in milliseconds. |
| `timer:elapsed_sec()` | `float` | Returns elapsed time in seconds. |
| `timer:is_running()` | `bool` | Returns true if currently running. |

---

## 9. Logging and Diagnostics

Outputs structured log entries to stdout and the Dear ImGui Log Console.

```lua
-- Standard log levels
OTLog.log("System initialized with status: %d", status)
OTLog.warn("Asset '%s' loaded with fallback settings", assetName)
OTLog.error("Failed to acquire entity handle")

-- Assert with engine error reporting
assert(entity ~= nil, "Entity must not be nil")
```
