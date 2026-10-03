add_library(OTTERUS_UTILITIES STATIC
    OtterusUtilities/RandomGenerator.cpp
    OtterusUtilities/RandomGenerator.h
    OtterusUtilities/SDL_wrappers.cpp
    OtterusUtilities/SDL_wrappers.h
    OtterusUtilities/Timer.cpp
    OtterusUtilities/Timer.h
)

target_include_directories(OTTERUS_UTILITIES PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/OtterusUtilities"
)

target_link_libraries(OTTERUS_UTILITIES PUBLIC
    SDL2::SDL2
    SDL2::SDL2_mixer
)
