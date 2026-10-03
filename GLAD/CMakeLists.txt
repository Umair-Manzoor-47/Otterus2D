add_library(GLAD STATIC
    glad/src/glad.c
    glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h
)

target_include_directories(GLAD PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/glad/include"
)
