<p align="center">
  <img src="docs/media/Otterus2D_SplashBanner.png" alt="Otterus2D Logo" width="600">
</p>

<p align="center">
  A modular 2D game engine built from scratch in <strong>C++20</strong> and <strong>OpenGL 4.5 Core</strong>, featuring an EnTT-based ECS, Box2D physics, and Lua scripting.
</p>

<p align="center">
  <a href="#quick-start"><img src="https://img.shields.io/badge/Platform-Windows%20x64-0078D6?logo=windows&logoColor=white" alt="Windows x64"></a>
  <a href="#tech-stack"><img src="https://img.shields.io/badge/C%2B%2B-20-00599C?logo=c%2B%2B&logoColor=white" alt="C++20"></a>
  <a href="#tech-stack"><img src="https://img.shields.io/badge/OpenGL-4.5%20Core-5586A4?logo=opengl&logoColor=white" alt="OpenGL 4.5"></a>
  <a href="#tech-stack"><img src="https://img.shields.io/badge/Dependencies-vcpkg-5C2D91?logo=vcpkg&logoColor=white" alt="vcpkg"></a>
  <a href="#tech-stack"><img src="https://img.shields.io/badge/Scripting-Lua%20%2B%20Sol3-000080?logo=lua&logoColor=white" alt="Lua + Sol3"></a>
  <a href="#tech-stack"><img src="https://img.shields.io/badge/Physics-Box2D-E84D3D" alt="Box2D"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/License-MIT-green.svg" alt="License MIT"></a>
</p>

<p align="center">
  <img src="docs/media/CozyScene.gif" alt="Cozy scene running on Otterus2D" width="720">
  <br>
  <em>Showcase scene running on Otterus2D: tilemap, animated sprites, particle weather, and spatial audio driven by Lua scripts.</em>
</p>

---

## Highlights

| Area | What's included |
| :--- | :--- |
| **Rendering** | Dynamic batching, layer-sorted sprites, primitives (rect, circle, line), TrueType text, 2D camera, framebuffers |
| **ECS** | EnTT registry with transform, sprite, animation, collider, physics, text, and script components |
| **Scripting** | Lua access to entities, input, assets, timers, random, game state stack, and follow camera |
| **Physics** | Static, kinematic, and dynamic bodies, sensors and triggers, runtime collider debug view |
| **Audio** | Streamed music (MP3) and multi-channel sound effects (WAV) via SDL2_mixer |
| **Assets** | Cached textures, shaders, fonts, and audio managed through a central `AssetManager` |
| **Tooling** | ImGui editor shell, scene viewport, filterable log console, structured logger with source locations |

---

## Documentation

Comprehensive documentation is available in the [Otterus2D Documentation Repository](https://github.com/Umair-Manzoor-47/Otterus2D-docs):

- **[Getting Started Guide](https://github.com/Umair-Manzoor-47/Otterus2D-docs)**: Environment setup, engine concepts, and first project walkthrough.
- **[Architecture & Internals](https://github.com/Umair-Manzoor-47/Otterus2D-docs)**: Subsystem boundaries, static library layering, and frame execution lifecycle.
- **[ECS Components & Lua API Reference](https://github.com/Umair-Manzoor-47/Otterus2D-docs)**: Complete component specifications and Sol3 scripting interface.
- **[How-To Recipes](https://github.com/Umair-Manzoor-47/Otterus2D-docs)**: Practical guides for collision handling, audio playback, and custom cameras.

---

## Quick Start

### Prerequisites

- **OS**: Windows 10/11 (64-bit)
- **Compiler**: Visual Studio 2022+ (MSVC v143+) with C++20 support
- **Build System**: CMake 3.20+
- **Package Manager**: [vcpkg](https://github.com/microsoft/vcpkg) (set `VCPKG_ROOT` environment variable, or use the automated build script)
- **Graphics**: GPU with OpenGL 4.5 support

All third-party dependencies (`SDL2`, `Box2D`, `EnTT`, `fmt`, `GLM`, `Lua`, `sol2`, `rapidjson`, `tinyfiledialogs`) are fully automated via `vcpkg.json` manifest mode. No manual downloads, release zips, or vendor folders are required.

---

### Option 1: One-Click Automated Build (Recommended)

Simply clone and run `build.bat`. If vcpkg is not already installed on your system, the script will automatically bootstrap it for you:

```powershell
git clone https://github.com/Umair-Manzoor-47/Otterus2D.git
cd Otterus2D

.\build.bat
```

To build in Debug mode instead:
```powershell
.\build.bat Debug
```

---

### Option 2: Command Line (CMake + vcpkg)

If you already have vcpkg configured on your system:

```powershell
git clone https://github.com/Umair-Manzoor-47/Otterus2D.git
cd Otterus2D

# Configure with vcpkg manifest mode
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="x64-windows"

# Build (Release or Debug)
cmake --build build --config Release
```

---

### Option 3: IDEs (Visual Studio / CLion)

#### Visual Studio 2022
1. Open Visual Studio &rarr; **Open a Local Folder** &rarr; Select `Otterus2D`.
2. Visual Studio will automatically detect `CMakeLists.txt` and `vcpkg.json`, resolving dependencies seamlessly.
3. Select `OTTERUS_EDITOR.exe` as the startup target and press **F5** to run.

#### CLion
1. Open the `Otterus2D` folder in CLion.
2. Go to **Settings &rarr; Build, Execution, Deployment &rarr; CMake**.
3. Under **CMake options**, set:
   ```
   -DCMAKE_TOOLCHAIN_FILE=path//to//%VCPKG_ROOT% -DVCPKG_TARGET_TRIPLET=x64-windows
   ```

4. Select `OTTERUS_EDITOR` and click **Run** (`Shift + F10`).

---

### Run

A post-build step automatically deploys all required runtime DLLs (`SDL2.dll`, `SDL2_mixer.dll`, `lua.dll`, audio codecs) directly adjacent to the executable.

```powershell
.\build\bin\Release\OTTERUS_EDITOR.exe
# or for Debug:
.\build\bin\Debug\OTTERUS_EDITOR.exe
```

---

## Libraries

| Library | Version | Managed via | Used for |
| :--- | :--- | :--- | :--- |
| **SDL2 / SDL2_mixer** | 2.32+ / 2.8+ | vcpkg | Windowing, input handling, and multi-channel audio |
| **Box2D** | 2.4.1 | vcpkg | 2D rigid-body physics simulation |
| **EnTT** | 3.16+ | vcpkg | Data-oriented Entity Component System (ECS) |
| **fmt** | 11.0+ | vcpkg | High-speed structured logging |
| **Lua + Sol3 (Sol2)** | 5.4+ / 3.5+ | vcpkg | Embedded scripting engine and C++ bindings |
| **GLM** | 1.0.3+ | vcpkg | OpenGL Mathematics library |
| **RapidJSON** | Latest | vcpkg | Fast JSON parser and generator for serialization |
| **tinyfiledialogs** | 3.21+ | vcpkg | Cross-platform native OS file dialogs |
| **Dear ImGui** | Docking branch | In-tree | Editor UI, dockspaces, framebuffers, inspect panels |
| **GLAD** | OpenGL 4.5 Core | In-tree | OpenGL function pointer loader |
| **SOIL / stb** | In-tree | In-tree | Image decoding and texture loading |

---

## Roadmap

**Status:** active development

- [ ] UI system (layout containers, buttons, sliders, input fields)
- [ ] Editor panels: scene hierarchy, component inspector, asset browser
- [ ] Decouple editor from engine
- [ ] Linux build support
- [ ] Automated tests and CI

---

## Links

- **Author:** Umair Manzoor
<!-- ... - **Devlog:** `[TODO: YouTube link]` ...-->
- **LinkedIn:** [Umair Manzoor](https://www.linkedin.com/in/umair-manzoor/)
- **Issues:** [github.com/Umair-Manzoor-47/Otterus2D/issues](https://github.com/Umair-Manzoor-47/Otterus2D/issues)
- **License:** [MIT](LICENSE)
