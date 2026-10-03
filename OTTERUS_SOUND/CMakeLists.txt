add_library(OTTERUS_SOUND STATIC
    src/Sounds/Essentials/Music.cpp
    src/Sounds/Essentials/Music.h
    src/Sounds/Essentials/SoundFX.cpp
    src/Sounds/Essentials/SoundFX.h
    src/Sounds/Essentials/SoundParams.h
    src/Sounds/MusicPlayer/MusicPlayer.cpp
    src/Sounds/MusicPlayer/MusicPlayer.h
    src/Sounds/SoundPlayer/SoundFxPlayer.cpp
    src/Sounds/SoundPlayer/SoundFxPlayer.h
)

target_include_directories(OTTERUS_SOUND PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(OTTERUS_SOUND PUBLIC
    OTTERUS_LOGGER
    OTTERUS_UTILITIES
    SDL2::SDL2
    SDL2::SDL2_mixer
)
