add_library(OTTERUS_WINDOW STATIC
    Windowing/Inputs/Button.cpp
    Windowing/Inputs/Button.h
    Windowing/Inputs/Keyboard.cpp
    Windowing/Inputs/Keyboard.h
    Windowing/Inputs/Keys.h
    Windowing/Inputs/Mouse.cpp
    Windowing/Inputs/Mouse.h
    Windowing/Inputs/MouseButtons.h
    Windowing/window/window.cpp
    Windowing/window/window.h
)

target_include_directories(OTTERUS_WINDOW PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(OTTERUS_WINDOW PUBLIC
    OTTERUS_LOGGER
    OTTERUS_UTILITIES
    SDL2::SDL2
)
