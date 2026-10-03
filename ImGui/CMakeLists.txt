add_library(ImGui STATIC
    src/backends/imgui_impl_opengl3.cpp
    src/backends/imgui_impl_opengl3.h
    src/backends/imgui_impl_opengl3_loader.h
    src/backends/imgui_impl_sdl2.cpp
    src/backends/imgui_impl_sdl2.h
    src/imconfig.h
    src/imgui.cpp
    src/imgui.h
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_internal.h
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
    src/imstb_rectpack.h
    src/imstb_textedit.h
    src/imstb_truetype.h
)

target_include_directories(ImGui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(ImGui PUBLIC
    SDL2::SDL2
    OpenGL::GL
)
