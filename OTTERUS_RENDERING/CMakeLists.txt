add_library(OTTERUS_RENDERING STATIC
    Rendering/Buffers/Framebuffer.cpp
    Rendering/Buffers/Framebuffer.h
    Rendering/Core/Batcher.h
    Rendering/Core/BatchRenderer.cpp
    Rendering/Core/BatchRenderer.h
    Rendering/Core/Camera2D.cpp
    Rendering/Core/Camera2D.h
    Rendering/Core/CircleBatchRenderer.cpp
    Rendering/Core/CircleBatchRenderer.h
    Rendering/Core/LineBatchRenderer.cpp
    Rendering/Core/LineBatchRenderer.h
    Rendering/Core/RectBatchRenderer.cpp
    Rendering/Core/RectBatchRenderer.h
    Rendering/Core/Renderer.cpp
    Rendering/Core/Renderer.h
    Rendering/Core/TextBatchRenderer.cpp
    Rendering/Core/TextBatchRenderer.h
    Rendering/Essentials/BatchTypes.h
    Rendering/Essentials/Font.cpp
    Rendering/Essentials/Font.h
    Rendering/Essentials/FontLoader.cpp
    Rendering/Essentials/FontLoader.h
    Rendering/Essentials/Primitives.h
    Rendering/Essentials/Shader.cpp
    Rendering/Essentials/Shader.h
    Rendering/Essentials/ShaderLoader.cpp
    Rendering/Essentials/ShaderLoader.h
    Rendering/Essentials/Texture.cpp
    Rendering/Essentials/Texture.h
    Rendering/Essentials/TextureLoader.cpp
    Rendering/Essentials/TextureLoader.h
    Rendering/Essentials/Vertex.h
)

target_include_directories(OTTERUS_RENDERING PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(OTTERUS_RENDERING PUBLIC
    GLAD
    SOIL
    OTTERUS_LOGGER
    GLM::GLM
    OpenGL::GL
)
