add_library(OTTERUS_UTILITIES STATIC
    src/OtterusUtilities/RandomGenerator.cpp
    src/OtterusUtilities/SDL_wrappers.cpp
    src/OtterusUtilities/Timer.cpp
    include/OtterusUtilities/OtterusUtilities.h
    include/OtterusUtilities/RandomGenerator.h
    include/OtterusUtilities/SDL_wrappers.h
    include/OtterusUtilities/Timer.h
)

target_include_directories(OTTERUS_UTILITIES PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(OTTERUS_UTILITIES PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/OtterusUtilities"
)

target_link_libraries(OTTERUS_UTILITIES PUBLIC
    SDL2::SDL2
    SDL2::SDL2_mixer
)
