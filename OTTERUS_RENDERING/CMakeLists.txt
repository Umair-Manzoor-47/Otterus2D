add_library(OTTERUS_RENDERING STATIC
    src/Rendering/Buffers/Framebuffer.cpp
    src/Rendering/Core/BatchRenderer.cpp
    src/Rendering/Core/Camera2D.cpp
    src/Rendering/Core/CircleBatchRenderer.cpp
    src/Rendering/Core/LightBatchRenderer.cpp
    src/Rendering/Core/LineBatchRenderer.cpp
    src/Rendering/Core/RectBatchRenderer.cpp
    src/Rendering/Core/Renderer.cpp
    src/Rendering/Core/TextBatchRenderer.cpp
    src/Rendering/Essentials/Font.cpp
    src/Rendering/Essentials/FontLoader.cpp
    src/Rendering/Essentials/Shader.cpp
    src/Rendering/Essentials/ShaderLoader.cpp
    src/Rendering/Essentials/Texture.cpp
    src/Rendering/Essentials/TextureLoader.cpp

    include/Rendering/Buffers/Framebuffer.h
    include/Rendering/Core/Batcher.h
    include/Rendering/Core/BatchRenderer.h
    include/Rendering/Core/Camera2D.h
    include/Rendering/Core/CircleBatchRenderer.h
    include/Rendering/Core/LightBatchRenderer.h
    include/Rendering/Core/LineBatchRenderer.h
    include/Rendering/Core/RectBatchRenderer.h
    include/Rendering/Core/Renderer.h
    include/Rendering/Core/TextBatchRenderer.h
    include/Rendering/Essentials/BatchTypes.h
    include/Rendering/Essentials/Font.h
    include/Rendering/Essentials/FontLoader.h
    include/Rendering/Essentials/Primitives.h
    include/Rendering/Essentials/SceneLighting.h
    include/Rendering/Essentials/Shader.h
    include/Rendering/Essentials/ShaderLoader.h
    include/Rendering/Essentials/Texture.h
    include/Rendering/Essentials/TextureLoader.h
    include/Rendering/Essentials/Vertex.h
)

target_include_directories(OTTERUS_RENDERING PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(OTTERUS_RENDERING PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Rendering"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Rendering/Buffers"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Rendering/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Rendering/Essentials"
)

target_link_libraries(OTTERUS_RENDERING PUBLIC
    GLAD
    SOIL
    OTTERUS_LOGGER
    GLM::GLM
    OpenGL::GL
)
