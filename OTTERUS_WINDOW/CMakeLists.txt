add_library(OTTERUS_WINDOW STATIC
    src/Windowing/Inputs/Button.cpp
    src/Windowing/Inputs/Keyboard.cpp
    src/Windowing/Inputs/Mouse.cpp
    src/Windowing/window/window.cpp
    include/Windowing/Inputs/Button.h
    include/Windowing/Inputs/Keyboard.h
    include/Windowing/Inputs/Keys.h
    include/Windowing/Inputs/Mouse.h
    include/Windowing/Inputs/MouseButtons.h
    include/Windowing/window/window.h
)

target_include_directories(OTTERUS_WINDOW PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(OTTERUS_WINDOW PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Windowing"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Windowing/Inputs"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Windowing/window"
)

target_link_libraries(OTTERUS_WINDOW PUBLIC
    OTTERUS_LOGGER
    OTTERUS_UTILITIES
    SDL2::SDL2
)
