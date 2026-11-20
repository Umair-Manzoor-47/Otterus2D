cmake_minimum_required(VERSION 3.20)

project(Otterus2D VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/MP)
endif()

add_compile_definitions(NOMINMAX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Dependencies
find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Compatibility Target Aliases
if(TARGET SDL2_mixer::SDL2_mixer AND NOT TARGET SDL2::SDL2_mixer)
    add_library(SDL2::SDL2_mixer ALIAS SDL2_mixer::SDL2_mixer)
endif()

if(TARGET box2d::box2d AND NOT TARGET Box2D::Box2D)
    add_library(Box2D::Box2D ALIAS box2d::box2d)
endif()

if(NOT TARGET Lua53::Lua53)
    add_library(Lua53::Lua53 INTERFACE IMPORTED GLOBAL)
    set_target_properties(Lua53::Lua53 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
    )
endif()

get_target_property(_entt_inc EnTT::EnTT INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(EnTT::EnTT INTERFACE "${_entt_inc}/entt")

if(TARGET glm::glm AND NOT TARGET GLM::GLM)
    add_library(GLM::GLM ALIAS glm::glm)
endif()

if(TARGET sol2::sol2 AND NOT TARGET Sol3::Sol3)
    add_library(Sol3::Sol3 ALIAS sol2::sol2)
endif()

if(TARGET tinyfiledialogs::tinyfiledialogs AND NOT TARGET tinyfiledialogs)
    add_library(tinyfiledialogs ALIAS tinyfiledialogs::tinyfiledialogs)
endif()

if(TARGET RapidJSON AND NOT TARGET RapidJSON::RapidJSON)
    add_library(RapidJSON::RapidJSON ALIAS RapidJSON)
endif()

# Engine Subprojects
add_subdirectory(GLAD)
add_subdirectory(SOIL)
add_subdirectory(ImGui)
add_subdirectory(OTTERUS_LOGGER)
add_subdirectory(OTTERUS_UTILITIES)
add_subdirectory(OTTERUS_WINDOW)
add_subdirectory(OTTERUS_SOUND)
add_subdirectory(OTTERUS_RENDERING)
add_subdirectory(OTTERUS_PHYSICS)
add_subdirectory(OTTERUS_FILESYSTEM)
add_subdirectory(OTTERUS_CORE)
add_subdirectory(OTTERUS_EDITOR)
