# How to Detect and Handle Collisions

Otterus2D provides two collision architectures:

1. **Event-Driven Physics Contacts (`ContactListener` and `PhysicsComponent`)**: Box2D resolves physics and dispatches contact data containing entity IDs, tags, groups, and trigger states.
2. **Arcade Geometric Queries (`CircleCollider` and `BoxCollider`)**: Manual spatial checks across entity sets without rigid body dynamics.

---

## 1. Event-Driven Collision Callbacks (Box2D)

When entities have a `PhysicsComponent`, Box2D simulates their motion and contacts. The engine's `ContactListener` extracts fixture data into `ObjectData` structures.

### The Contact Object (`ObjectData`)

Each colliding object in a contact callback provides the following fields:

| Field | Type | Description |
| :--- | :--- | :--- |
| `object.entityId` | `int` | The entity integer ID. Pass to `Entity(id)` to acquire handle. |
| `object.tag` | `string` | Tag assigned in `PhysicsAttributes.objectData.tag`. |
| `object.group` | `string` | Group assigned in `PhysicsAttributes.objectData.group`. |
| `object.isTrigger` | `bool` | True if collider was configured as a sensor. |
| `object.isCollider` | `bool` | True if collider participates in solid physical collisions. |

### Setup: Creating Physical Entities with ObjectData

```lua
local attr = PhysicsAttributes.new()
attr.type = BodyType.DYNAMIC
attr.density = 1.0
attr.boxShape = true
attr.boxSize = vec2(32.0, 32.0)
attr.fixedRotation = true

-- Configure contact metadata
attr.objectData = ObjectData.new({
    tag = "Player",
    group = "Actors",
    isCollider = true,
    isTrigger = false,
    entityID = player:id()
})

player:add_component(PhysicsComponent.new(attr))
```

### Polling Contacts and Dispatching Callbacks

In your update loop, query `ContactListener.get_user_data()` to receive the active collision pair:

```lua
function UpdateCollisions()
    local object_a, object_b = ContactListener.get_user_data()
    if object_a and object_b then
        OnCollision(object_a, object_b)
    end
end

function OnCollision(a, b)
    -- Check if player collided with a hazard trigger
    if a.isTrigger and b.group == "Actors" then
        HandleTrigger(a, b)
    elseif b.isTrigger and a.group == "Actors" then
        HandleTrigger(b, a)
    end
end

function HandleTrigger(trigger, actor)
    OTLog.log("Trigger [%s] activated by Actor [%s]", trigger.tag, actor.tag)
    
    -- Acquire entity handles via entityId
    local actorEntity = Entity(actor.entityId)
    local physics = actorEntity:get_component(PhysicsComponent)
    if physics then
        physics:linear_impulse(vec2(0.0, -10.0)) -- Bounce actor upwards
    end
end
```

---

## 2. The Deferred Destruction Rule

> **Critical Rule**: Never call `entity:kill()` or mutate physics bodies directly inside a collision callback or during `Registry.get_entities():for_each()` iteration.

Box2D and EnTT lock internal state while simulating and iterating:
1. Store target entity IDs in a table (`local to_kill = {}`).
2. Complete the simulation / collision callback step.
3. Iterate through `to_kill` and execute `Entity(id):kill()`.
