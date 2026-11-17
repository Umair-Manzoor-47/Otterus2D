add_library(OTTERUS_PHYSICS STATIC
    src/Physics/Box2Dwrappers.cpp
    src/Physics/ContactListener.cpp
    src/Physics/UserData.cpp
    include/Physics/Box2Dwrappers.h
    include/Physics/ContactListener.h
    include/Physics/UserData.h
)

target_include_directories(OTTERUS_PHYSICS PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(OTTERUS_PHYSICS PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Physics"
)

target_link_libraries(OTTERUS_PHYSICS PUBLIC
    OTTERUS_LOGGER
    Box2D::Box2D
)
