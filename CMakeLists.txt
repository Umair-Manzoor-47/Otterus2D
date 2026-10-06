cmake_minimum_required(VERSION 3.20)
project(Otterus2D VERSION 1.0.0 LANGUAGES C CXX)


# Standard C++ and C settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configure MSVC runtime library to match prebuilt static dependencies (/MT and /MTd)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/MP)
endif()

# Global preprocessor definitions
add_compile_definitions(NOMINMAX)

# Default output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Find OpenGL
find_package(OpenGL REQUIRED)

# -----------------------------------------------------------------------------
# Third-party Dependencies
# -----------------------------------------------------------------------------

# SDL2
add_library(SDL2::SDL2 STATIC IMPORTED GLOBAL)
set_target_properties(SDL2::SDL2 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/SDL/include"
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/SDL2.lib"
)

# SDL2_mixer
add_library(SDL2::SDL2_mixer STATIC IMPORTED GLOBAL)
set_target_properties(SDL2::SDL2_mixer PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/SDL/include"
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/SDL2_mixer.lib"
)

# Box2D
add_library(Box2D::Box2D STATIC IMPORTED GLOBAL)
set_target_properties(Box2D::Box2D PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/box2d/include"
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/box2d/lib/release/box2d.lib"
    IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/box2d/lib/debug/box2d.lib"
    IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/box2d/lib/release/box2d.lib"
)

# Lua 5.3
add_library(Lua53::Lua53 STATIC IMPORTED GLOBAL)
set_target_properties(Lua53::Lua53 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/lua53/include"
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/lua53/lib/lua53.lib"
)

# Header-only: EnTT
add_library(EnTT::EnTT INTERFACE IMPORTED GLOBAL)
set_target_properties(EnTT::EnTT PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/entt"
)

# Header-only: GLM
add_library(GLM::GLM INTERFACE IMPORTED GLOBAL)
set_target_properties(GLM::GLM PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/glm"
)

# Header-only: Sol3
add_library(Sol3::Sol3 INTERFACE IMPORTED GLOBAL)
set_target_properties(Sol3::Sol3 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/sol3"
)

# -----------------------------------------------------------------------------
# Subprojects (in dependency order)
# -----------------------------------------------------------------------------
add_subdirectory(GLAD)
add_subdirectory(SOIL)
add_subdirectory(ImGui)
add_subdirectory(OTTERUS_LOGGER)
add_subdirectory(OTTERUS_UTILITIES)
add_subdirectory(OTTERUS_WINDOW)
add_subdirectory(OTTERUS_SOUND)
add_subdirectory(OTTERUS_RENDERING)
add_subdirectory(OTTERUS_PHYSICS)
add_subdirectory(OTTERUS_CORE)
add_subdirectory(OTTERUS_EDITOR)
