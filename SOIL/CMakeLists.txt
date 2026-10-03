add_library(SOIL STATIC
    src/image_dxt.c
    src/image_helper.c
    src/soil.c
    src/stb_image_aug.c
    include/SOIL/image_dxt.h
    include/SOIL/image_helper.h
    include/SOIL/SOIL.h
    include/SOIL/stbi_dds_aug.h
    include/SOIL/stbi_dds_aug_c.h
    include/SOIL/stb_image_aug.h
    include/SOIL/stb_truetype.h
)

target_include_directories(SOIL PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(SOIL PRIVATE
    _CRT_SECURE_NO_WARNINGS
)

if(MSVC)
    target_compile_options(SOIL PRIVATE /wd4244 /wd4018 /wd4267)
endif()
