add_executable(OTTERUS_EDITOR
    src/Application.cpp
    src/Application.h
    src/editor/displays/IDisplay.h
    src/editor/displays/LogDisplay.cpp
    src/editor/displays/LogDisplay.h
    src/editor/displays/SceneDisplay.cpp
    src/editor/displays/SceneDisplay.h
    src/main.cpp
)

target_include_directories(OTTERUS_EDITOR PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(OTTERUS_EDITOR PRIVATE
    NOMINMAX
)

target_link_libraries(OTTERUS_EDITOR PRIVATE
    OTTERUS_CORE
    ImGui
    OpenGL::GL
)


# Copy runtime DLLs to output directory
set(SDL_RUNTIME_DLLS
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/SDL2.dll"
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/SDL2_mixer.dll"
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/optional/libmodplug-1.dll"
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/optional/libogg-0.dll"
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/optional/libopus-0.dll"
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL/lib/optional/libopusfile-0.dll"
)

add_custom_command(TARGET OTTERUS_EDITOR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL_RUNTIME_DLLS}
        "$<TARGET_FILE_DIR:OTTERUS_EDITOR>"
    COMMENT "Copying runtime DLLs to output directory..."
)

# Copy assets directory to output directory so binary can be run standalone
add_custom_command(TARGET OTTERUS_EDITOR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:OTTERUS_EDITOR>/assets"
    COMMENT "Copying assets to output directory..."
)
