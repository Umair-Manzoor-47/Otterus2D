# AssetManager Reference

The `AssetManager` provides centralized loading, caching, and lifecycle management for GPU textures, GLSL shaders, TrueType fonts, and audio tracks in Otterus2D.

---

## 1. Supported Asset Formats

| Asset Type | Supported Formats | Underlying Loader | GPU / Memory Storage |
| :--- | :--- | :--- | :--- |
| **Textures** | `.png`, `.jpg`, `.bmp` | SOIL / stb_image | OpenGL 2D Texture (`GLuint`) |
| **Shaders** | `.vert`, `.frag`, `.glsl` | Modern OpenGL 4.5 Core | Compiled GL Program ID (`GLuint`) |
| **Fonts** | `.ttf`, `.otf` | TrueType Font Atlas | Texture Atlas & Glyph Metric Map |
| **Music (Streaming)** | `.mp3`, `.ogg`, `.wav` | SDL2_mixer (`Mix_Music`) | Streamed from disk / memory |
| **Sound Effects (SFX)** | `.wav`, `.ogg` | SDL2_mixer (`Mix_Chunk`) | Pre-loaded audio chunk buffer |

---

## 2. Texture Filtering: Pixel Art vs. Linear

When loading textures, the `pixel_art` boolean flag determines the OpenGL texture minification and magnification filters:

```lua
-- Pixel art: Nearest-Neighbor filtering (crisp, pixel-perfect edges)
AssetManager.add_texture("character", "assets/textures/character.png", true)

-- Smooth: Bilinear filtering (smooth gradients, high-res UI)
AssetManager.add_texture("background", "assets/textures/bg.png", false)
```

| Setting | `pixel_art = true` | `pixel_art = false` |
| :--- | :--- | :--- |
| **GL_TEXTURE_MIN_FILTER** | `GL_NEAREST` | `GL_LINEAR` |
| **GL_TEXTURE_MAG_FILTER** | `GL_NEAREST` | `GL_LINEAR` |
| **Ideal For** | Retro sprites, tilemaps, pixel fonts | High-resolution art, soft backgrounds |

---

## 3. Lua API Methods

```lua
-- Textures
local ok = AssetManager.add_texture(name, path, pixel_art)

-- Audio
local ok = AssetManager.add_music(name, path)
local ok = AssetManager.add_sound(name, path)

-- Fonts (path, size in points)
local ok = AssetManager.add_font(name, path, 32.0)
```

All methods return `true` on successful load and return `false` if the file was missing, unreadable, or already registered under that key.

---

## 4. Cache Management and Memory Lifecycle

- **Keyed Storage**: Assets are stored in `std::map<std::string, std::shared_ptr<T>>` indexed by unique string identifiers.
- **Deduplication**: Calling `add_texture` or `add_sound` with an existing name is safely ignored; the existing resource is preserved.
- **Deallocation**: Resources remain in memory until the application shuts down or the scene unloads, triggering RAII cleanup (`glDeleteTextures`, `glDeleteProgram`, `Mix_FreeChunk`, `Mix_FreeMusic`).
