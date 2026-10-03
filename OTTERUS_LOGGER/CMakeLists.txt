add_library(OTTERUS_LOGGER STATIC
    Logger/Logger.cpp
    Logger/Logger.h
    Logger/Logger.inl
)

target_include_directories(OTTERUS_LOGGER PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Logger"
)
