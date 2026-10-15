# Lua Scripting API Reference

Otterus2D exposes core engine services, ECS entities, input devices, and resource loaders to Lua 5.3 via Sol3 bindings.

---

## 1. Script Entry Points

Gameplay scripts are loaded from `./assets/scripts/main.lua`. The engine requires a global `main` table containing update and render functions:

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

| Signature | Description |
| :--- | :--- |
| `Entity(name: string, group: string) -> Entity` | Instantiates a new entity in the registry with name and group tags. |
| `Entity(id: int) -> Entity` | Wraps an existing integer entity ID into an operable script handle. |
| `entity:add_component(comp: Component) -> Component` | Attaches a component instance and returns its reference. |
| `entity:get_component(Type: ComponentType) -> Component?` | Returns attached component instance, or `nil` if not present. |
| `entity:has_component(Type: ComponentType) -> bool` | Checks if the entity possesses the specified component type. |
| `entity:remove_component(Type: ComponentType) -> void` | Detaches the component type from the entity. |
| `entity:name() -> string` | Returns the entity's identification name tag. |
| `entity:group() -> string` | Returns the entity's group categorization tag. |
| `entity:id() -> int` | Returns the underlying integer entity identifier. |
| `entity:kill() -> void` | Destroys the entity and purges all its component data. |

### Registry (`Registry`)

| Signature | Description |
| :--- | :--- |
| `Registry.get_entities(...Type: ComponentType) -> runtime_view` | Queries entities possessing all specified component types. |
| `view:for_each(callback: function(ent: Entity)) -> void` | Iterates over each entity matching the view criteria. |
| `view:exclude(...Type: ComponentType) -> void` | Filters out entities possessing any of the excluded types. |
| `Registry.clear() -> void` | Purges all entities and components from the active scene registry. |

---

## 3. Input System

Input states are queried through static `Keyboard` and `Mouse` user types.

### Keyboard (`Keyboard`)

| Signature | Description |
| :--- | :--- |
| `Keyboard.pressed(key: KeyConstant) -> bool` | Returns true continuously while key is held down. |
| `Keyboard.just_pressed(key: KeyConstant) -> bool` | Returns true only on the initial frame the key is pressed. |
| `Keyboard.just_released(key: KeyConstant) -> bool` | Returns true only on the frame the key is released. |

#### Key Constants
- Letters: `KEY_A` through `KEY_Z`
- Numbers: `KEY_0` through `KEY_9`
- Arrows: `KEY_UP`, `KEY_DOWN`, `KEY_LEFT`, `KEY_RIGHT`
- Modifiers: `KEY_LCTRL`, `KEY_RCTRL`, `KEY_LSHIFT`, `KEY_RSHIFT`, `KEY_LALT`, `KEY_RALT`
- Controls: `KEY_SPACE`, `KEY_ENTER`, `KEY_ESC`, `KEY_BACKSPACE`
- Functions: `KEY_F1` through `KEY_F12`

### Mouse (`Mouse`)

| Signature | Description |
| :--- | :--- |
| `Mouse.pressed(btn: MouseButton) -> bool` | Returns true continuously while mouse button is held down. |
| `Mouse.just_pressed(btn: MouseButton) -> bool` | Returns true only on the initial frame the button is pressed. |
| `Mouse.just_released(btn: MouseButton) -> bool` | Returns true only on the frame the button is released. |
| `Mouse.screen_position() -> vec2` | Returns pixel coordinates relative to the window backbuffer. |
| `Mouse.world_position() -> vec2` | Returns coordinates transformed into 2D world space. |
| `Mouse.wheel_x() -> int` | Returns horizontal scroll wheel delta for the current frame. |
| `Mouse.wheel_y() -> int` | Returns vertical scroll wheel delta for the current frame. |
| `Mouse.is_moving() -> bool` | Returns true if the mouse pointer moved during the frame. |

#### Mouse Constants
`LEFT_BTN`, `MIDDLE_BTN`, `RIGHT_BTN`

---

## 4. Physics Contacts (`ContactListener`)

| Signature | Description |
| :--- | :--- |
| `ContactListener.get_user_data() -> ObjectData?, ObjectData?` | Returns active colliding object pair, or `nil` if no contact occurred. |

### Contact Metadata Fields (`ObjectData`)
- `object.tag`: String tag identifier.
- `object.group`: String group identifier.
- `object.entityId`: Integer ID of the entity (pass to `Entity(id)`).
- `object.isTrigger`: True if configured as a sensor.
- `object.isCollider`: True if participating in solid physical collisions.

---

## 5. Audio Subsystem

### Music (`Music`)

| Signature | Description |
| :--- | :--- |
| `Music.play(name: string, [loops: int = -1]) -> void` | Streams background music track (`-1` loops indefinitely). |
| `Music.stop() -> void` | Stops active music playback. |
| `Music.pause() -> void` | Temporarily halts music playback. |
| `Music.resume() -> void` | Resumes paused music playback. |
| `Music.set_volume(volume: int) -> void` | Adjusts music volume on a scale from 0 to 100. |
| `Music.is_playing() -> bool` | Returns true if music is actively streaming. |

### Sound Effects (`Sound`)

| Signature | Description |
| :--- | :--- |
| `Sound.play(name: string, [loops: int = 0, channel: int = -1]) -> void` | Plays sound effect on designated channel (`-1` selects first free). |
| `Sound.stop(channel: int) -> void` | Stops playback on specified channel (`-1` stops all channels). |
| `Sound.set_volume(channel: int, volume: int) -> void` | Adjusts channel volume on a scale from 0 to 100. |
| `Sound.is_playing(channel: int) -> bool` | Returns true if the channel is currently playing audio. |

---

## 6. AssetManager (`AssetManager`)

| Signature | Description |
| :--- | :--- |
| `AssetManager.add_texture(name: string, path: string, pixel_art: bool) -> bool` | Loads texture into GPU memory (`pixel_art` toggles nearest vs linear). |
| `AssetManager.add_music(name: string, path: string) -> bool` | Registers streaming audio file (`.mp3`, `.ogg`, `.wav`). |
| `AssetManager.add_sound(name: string, path: string) -> bool` | Loads sound effect audio chunk into memory (`.wav`, `.ogg`). |
| `AssetManager.add_font(name: string, path: string, [size: float = 32.0]) -> bool` | Generates TrueType font atlas. |

---

## 7. Camera Control (`Camera`)

| Signature | Description |
| :--- | :--- |
| `Camera.position() -> vec2` | Returns current camera center in world coordinates. |
| `Camera.set_position(pos: vec2) -> void` | Moves camera center to target world position. |
| `Camera.scale() -> float` | Returns current camera zoom factor. |
| `Camera.set_scale(scale: float) -> void` | Sets camera zoom factor. |
| `Camera.width() -> int` | Returns viewport pixel width. |
| `Camera.height() -> int` | Returns viewport pixel height. |
| `Camera.get_world_coords(screen_pos: vec2) -> vec2` | Converts screen pixel coordinates to world coordinates. |
| `Camera.get_screen_coords(world_pos: vec2) -> vec2` | Converts world coordinates to screen pixel coordinates. |

---

## 8. Math and Utilities

### 2D Vector (`vec2`)

| Signature | Description |
| :--- | :--- |
| `vec2(scalar: float) -> vec2` | Constructs 2D vector with identical x and y values. |
| `vec2(x: float, y: float) -> vec2` | Constructs 2D vector with explicit x and y coordinates. |
| `v:length() -> float` | Returns Euclidean length of the vector. |
| `v:lengthSq() -> float` | Returns squared length of the vector (avoids square root). |

Standard arithmetic operators (`+`, `-`, `*`, `/`) are overloaded for vector-vector and vector-scalar operations.

### Free Math Functions

| Signature | Description |
| :--- | :--- |
| `distance(v1: vec2, v2: vec2) -> float` | Returns Euclidean distance between two points. |
| `lerp(a: float, b: float, t: float) -> float` | Computes linear interpolation between `a` and `b` by factor `t`. |
| `clamp(val: float, min: float, max: float) -> float` | Constrains value within `[min, max]` range. |

### Precision Timer (`Timer`)

| Signature | Description |
| :--- | :--- |
| `Timer() -> Timer` | Instantiates a new inactive timer. |
| `timer:start() -> void` | Begins tracking elapsed time. |
| `timer:stop() -> void` | Halts timer. |
| `timer:restart() -> void` | Halts and immediately restarts timer from zero. |
| `timer:pause() -> void` | Temporarily halts elapsed time accumulation. |
| `timer:resume() -> void` | Resumes a paused timer. |
| `timer:elapsed_ms() -> int` | Returns elapsed time in milliseconds. |
| `timer:elapsed_sec() -> float` | Returns elapsed time in seconds. |
| `timer:is_running() -> bool` | Returns true if the timer is actively ticking. |
| `timer:is_paused() -> bool` | Returns true if the timer is currently paused. |

---

## 9. Logging and Diagnostics

| Signature | Description |
| :--- | :--- |
| `OTLog.log(fmt: string, ...args) -> void` | Writes info message to stdout and editor Log Console. |
| `OTLog.warn(fmt: string, ...args) -> void` | Writes warning message to stdout and editor Log Console. |
| `OTLog.error(fmt: string, ...args) -> void` | Writes error message with source origin to Log Console. |
| `assert(cond: bool, [message: string]) -> bool` | Evaluates condition; writes to `OTLog.error` on failure. |
