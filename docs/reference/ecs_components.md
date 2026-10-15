# ECS Components Reference

In Otterus2D, components are plain data structures (PODs) managed by EnTT. They contain no gameplay logic; logic is handled exclusively by systems.

<p align="center">
  <img src="../media/diagrams/ecs_architecture.svg" alt="ECS Architecture Data Flow" width="100%">
</p>

---

## Component Summary

| Component | Purpose | Handled By System |
| :--- | :--- | :--- |
| `TransformComponent` | World position, scale, and rotation. | `RenderSystem`, `PhysicsSystem` |
| `SpriteComponent` | Texture rendering, dimensions, UV coordinates, layer sorting. | `RenderSystem` |
| `AnimationComponent` | Frame-based sprite sheet animation parameters. | `AnimationSystem` |
| `BoxColliderComponent` | Axis-aligned bounding box for overlap detection. | `PhysicsSystem` |
| `CircleColliderComponent` | Radial bounding area for overlap detection. | `PhysicsSystem` |
| `PhysicsComponent` | Box2D rigid body configuration and simulation handle. | `PhysicsSystem` |
| `RigidBodyComponent` | Simple non-Box2D kinematic velocity vector. | `PhysicsSystem` |
| `TextComponent` | TrueType font rendering, text string, and color. | `RenderUISystem` |
| `ScriptComponent` | Lua update and render callbacks. | `ScriptingSystem` |
| `Identification` | Entity name, group tag, and internal integer ID. | `Registry` |

---

## 1. TransformComponent

Defines spatial transformation in 2D world space.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `position` | `glm::vec2` | `(0.0, 0.0)` | World coordinates (x, y). |
| `scale` | `glm::vec2` | `(1.0, 1.0)` | Scaling factor along x and y axes. |
| `rotation` | `float` | `0.0` | Rotation angle in degrees. |

### Lua Usage

```lua
-- Add to an entity
local transform = entity:add_component(TransformComponent.new())
transform.position = glm.vec2.new(100.0, 200.0)
transform.scale = glm.vec2.new(2.0, 2.0)
transform.rotation = 45.0
```

---

## 2. SpriteComponent

Defines visual appearance using a cached texture from `AssetManager`.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `texture_name` | `string` | `""` | Key of the loaded texture in `AssetManager`. |
| `width` | `float` | `0.0` | Rendered width in world units. |
| `height` | `float` | `0.0` | Rendered height in world units. |
| `start_x` | `int` | `0` | Pixel X offset within sprite sheet. |
| `start_y` | `int` | `0` | Pixel Y offset within sprite sheet. |
| `layer` | `int` | `0` | Render sorting order (lower draws first). |
| `color` | `Color` | `(255, 255, 255, 255)` | RGBA tint multiplier. |

### Lua Usage

```lua
local sprite = entity:add_component(SpriteComponent.new(
    "player_texture", -- texture key
    32.0,             -- width
    32.0,             -- height
    0,                -- start_x
    0,                -- start_y
    1                 -- layer
))
```

---

## 3. AnimationComponent

Cycles UV coordinates of a `SpriteComponent` across frames in a sprite sheet.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `numFrames` | `int` | `1` | Total number of animation frames. |
| `frameRate` | `int` | `1` | Playback speed in frames per second. |
| `frameOffset` | `int` | `0` | Starting frame index. |
| `currentFrame` | `int` | `0` | Currently active frame index. |
| `vertical` | `bool` | `false` | True if frames are stacked vertically. |
| `looped` | `bool` | `false` | True to loop playback indefinitely. |

### Lua Usage

```lua
local anim = entity:add_component(AnimationComponent.new())
anim.numFrames = 6
anim.frameRate = 12
anim.looped = true
```

---

## 4. BoxColliderComponent

Defines a rectangular boundary for simple trigger or collision queries.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `width` | `int` | `1` | Box width in pixels/units. |
| `height` | `int` | `1` | Box height in pixels/units. |
| `offset` | `glm::vec2` | `(0.0, 0.0)` | Offset relative to entity transform. |
| `colliding` | `bool` | `false` | Flag set when an intersection occurs. |

### Lua Usage

```lua
local box = entity:add_component(BoxColliderComponent.new())
box.width = 32
box.height = 32
box.offset = glm.vec2.new(0.0, 0.0)
```

---

## 5. CircleColliderComponent

Defines a radial boundary for simple distance-based collision queries.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `radius` | `float` | `0.0` | Radius of the collision circle. |
| `offset` | `glm::vec2` | `(0.0, 0.0)` | Center offset relative to transform. |
| `colliding` | `bool` | `false` | Flag set when an intersection occurs. |

### Lua Usage

```lua
local circle = entity:add_component(CircleColliderComponent.new())
circle.radius = 16.0
```

---

## 6. PhysicsComponent

Wraps a Box2D physical body (`b2Body`) for full rigid-body dynamics and contacts.

### Attributes (`PhysicsAttributes`)

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `type` | `RigidbodyType` | `STATIC` | Body mode: `STATIC` (0), `KINEMATIC` (1), `DYNAMIC` (2). |
| `density` | `float` | `1.0` | Material mass density. |
| `friction` | `float` | `0.2` | Surface friction coefficient. |
| `restitution` | `float` | `0.2` | Bounciness (restitution coefficient). |
| `gravityScale` | `float` | `1.0` | Multiplier for global gravity. |
| `isSensor` | `bool` | `false` | If true, detects overlap without physical collision response. |
| `fixedRotation`| `bool` | `true` | Prevents rotational torque from external forces. |
| `boxShape` | `bool` | `true` | If true, uses box fixture; otherwise circle. |

### Lua Usage

```lua
local attr = PhysicsAttributes.new()
attr.type = RigidbodyType.DYNAMIC
attr.density = 1.0
attr.friction = 0.3
attr.boxShape = true
attr.boxSize = glm.vec2.new(32.0, 32.0)

local physics = entity:add_component(PhysicsComponent.new(attr))
```

---

## 7. RigidBodyComponent

Lightweight, non-Box2D kinematic velocity component for simple programmatic movement.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `velocity` | `glm::vec2` | `(0.0, 0.0)` | Linear velocity vector in world units per second. |

### Lua Usage

```lua
local rb = entity:add_component(RigidBodyComponent.new())
rb.velocity = glm.vec2.new(150.0, 0.0)
```

---

## 8. TextComponent

Renders TrueType text using a font loaded in `AssetManager`.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `textStr` | `string` | `""` | The string content to render. |
| `fontName` | `string` | `"pixel"` | Font identifier in `AssetManager`. |
| `padding` | `int` | `0` | Pixel spacing between characters. |
| `wrap` | `float` | `-1.0` | Max width before wrapping line (`-1.0` disables wrap). |
| `color` | `Color` | `(255, 255, 255, 255)` | Text color tint. |
| `isHidden` | `bool` | `false` | Toggle visibility without removing component. |

### Lua Usage

```lua
local text = entity:add_component(TextComponent.new())
text.textStr = "Score: 100"
text.fontName = "pixel"
```

---

## 9. ScriptComponent

Attaches Lua update and render hooks directly to an individual entity.

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
| `update` | `sol::protected_function` | Called once per frame during logic update. |
| `render` | `sol::protected_function` | Called once per frame during render pass. |

---

## 10. Identification

Stores metadata for debugging, inspector display, and entity grouping.

### Fields

| Field | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `name` | `string` | `"GameObject"` | Friendly identifier displayed in editor. |
| `group` | `string` | `""` | Category tag (e.g., `"Enemy"`, `"Projectile"`). |
| `entity_id` | `int32_t` | `-1` | Underlying integer entity index. |
