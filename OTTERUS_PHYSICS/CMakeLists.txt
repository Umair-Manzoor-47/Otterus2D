add_library(OTTERUS_PHYSICS STATIC
    Physics/Box2Dwrappers.cpp
    Physics/Box2Dwrappers.h
    Physics/ContactListener.cpp
    Physics/ContactListener.h
    Physics/UserData.cpp
    Physics/UserData.h
)

target_include_directories(OTTERUS_PHYSICS PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(OTTERUS_PHYSICS PUBLIC
    OTTERUS_LOGGER
    Box2D::Box2D
)
