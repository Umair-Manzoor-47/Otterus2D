# Otterus2D Documentation Portal

Welcome to the official developer documentation for the Otterus2D game engine.

The documentation is organized according to the Diataxis framework, dividing knowledge into four distinct categories based on reader intent:

---

## Documentation Structure

```text
docs/
├── architecture/       <-- [Explanation] System design, mental models, and internals
├── reference/          <-- [Reference] API specifications, ECS components, and syntax
├── tutorials/          <-- [Tutorials] Step-by-step hands-on guides for newcomers
└── how-to/             <-- [How-To] Recipes and solutions for specific tasks
```

---

### 1. Architecture and Internals (Explanation)
In-depth technical guides explaining engine design, lifecycle, and low-level subsystems.

- [Engine Architecture and System Overview](architecture/engine_overview.md)  
  Subsystem hierarchy, one-way dependency rules, the four-phase frame lifecycle, and the Lua-C++ bridge.

---

### 2. Reference Manuals (Reference)
Authoritative technical specifications and data dictionaries.

- [ECS Components Reference](reference/ecs_components.md)  
  Technical specification for all ten core components (`TransformComponent`, `SpriteComponent`, `BoxColliderComponent`, etc.).
- [Lua Scripting API Reference](reference/lua_api.md)  
  Complete listing of functions, tables, and user types exposed to Lua via Sol3.
- [AssetManager and Resource Caching](reference/asset_manager.md)  
  Texture, shader, font, and audio resource caching mechanics.

---

### 3. Tutorials (Learning-Oriented)
Hands-on walkthroughs designed to guide new developers from project setup to a working game.

- [Quickstart: Building a 2D Scene](tutorials/getting_started.md)  
  Creating a scene, instantiating an entity, attaching components, and binding user input.

---

### 4. How-To Guides (Problem-Oriented)
Focused procedures addressing specific engineering problems.

- [Detecting and Handling Collisions](how-to/handle_collisions.md)  
  Geometric queries with colliders, Box2D rigid bodies, and deferred destruction patterns.
- [Configuring Audio and Music Playback](how-to/play_audio_sfx.md)  
  Streaming background music and managing multi-channel sound effects.
