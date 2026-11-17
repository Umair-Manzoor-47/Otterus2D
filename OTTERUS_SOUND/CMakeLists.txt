add_library(OTTERUS_SOUND STATIC
    src/Sounds/Essentials/Music.cpp
    src/Sounds/Essentials/SoundFX.cpp
    src/Sounds/MusicPlayer/MusicPlayer.cpp
    src/Sounds/SoundPlayer/SoundFxPlayer.cpp
    include/Sounds/Essentials/Music.h
    include/Sounds/Essentials/SoundFX.h
    include/Sounds/Essentials/SoundParams.h
    include/Sounds/MusicPlayer/MusicPlayer.h
    include/Sounds/SoundPlayer/SoundFxPlayer.h
)

target_include_directories(OTTERUS_SOUND PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(OTTERUS_SOUND PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Sounds"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Sounds/Essentials"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Sounds/MusicPlayer"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Sounds/SoundPlayer"
)

target_link_libraries(OTTERUS_SOUND PUBLIC
    OTTERUS_LOGGER
    OTTERUS_UTILITIES
    SDL2::SDL2
    SDL2::SDL2_mixer
)
