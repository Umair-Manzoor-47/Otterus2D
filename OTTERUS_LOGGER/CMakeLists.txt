add_library(OTTERUS_LOGGER STATIC
    src/Logger/Logger.cpp
    include/Logger/Logger.h
    include/Logger/Logger.inl
)

target_include_directories(OTTERUS_LOGGER PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Logger"
)

target_link_libraries(OTTERUS_LOGGER PUBLIC
    fmt::fmt
)
