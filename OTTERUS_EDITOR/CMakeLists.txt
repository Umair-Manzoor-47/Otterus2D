add_executable(OTTERUS_EDITOR
    src/Application.cpp
    src/Application.h
    src/editor/systems/GridSystem.h
    src/editor/systems/GridSystem.cpp
    src/editor/displays/IDisplay.h
    src/editor/displays/LogDisplay.cpp
    src/editor/displays/LogDisplay.h
    src/editor/displays/SceneDisplay.cpp
    src/editor/displays/SceneDisplay.h
    src/editor/displays/TilesetDisplay.h
    src/editor/displays/TilesetDisplay.cpp
    src/editor/displays/TilemapDisplay.h
    src/editor/displays/TilemapDisplay.cpp
    src/editor/displays/AssetDisplay.h
    src/editor/displays/AssetDisplay.cpp
    src/editor/displays/MenuDisplay.h
    src/editor/displays/MenuDisplay.cpp
    src/editor/scene/SceneObject.h
    src/editor/scene/SceneObject.cpp
    src/editor/scene/SceneManager.h
    src/editor/scene/SceneManager.cpp
    src/editor/utilities/EditorUtilities.h
    src/editor/utilities/editor_textures.h
    src/editor/utilities/EditorFrameBuffer.h

    #Tools
    src/editor/tools/AbstractTool.h
    src/editor/tools/AbstractTool.cpp
    src/editor/tools/TileTool.h
    src/editor/tools/TileTool.cpp
    src/editor/tools/CreateTileTool.h
    src/editor/tools/CreateTileTool.cpp
    src/editor/tools/RectFillTool.h
    src/editor/tools/RectFillTool.cpp
    src/editor/tools/ToolAccessories.h
    src/editor/tools/ToolManager.h
    src/editor/tools/ToolManager.cpp

    src/main.cpp
)

target_include_directories(OTTERUS_EDITOR PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(OTTERUS_EDITOR PRIVATE
    OTTERUS_CORE
    OTTERUS_FILESYSTEM
    ImGui
)


# Copy runtime DLLs to output directory
add_custom_command(TARGET OTTERUS_EDITOR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:OTTERUS_EDITOR>
        "$<TARGET_FILE_DIR:OTTERUS_EDITOR>"
    COMMAND_EXPAND_LISTS
    COMMENT "Copying runtime DLLs to output directory..."
)
set_target_properties(OTTERUS_EDITOR PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
