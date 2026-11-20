add_library(OTTERUS_FILESYSTEM STATIC
    include/OtterusFileSystem/Dialogs/FileDialog.h
    src/FileDialog.cpp
)

target_include_directories(OTTERUS_FILESYSTEM PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(OTTERUS_FILESYSTEM 
    PUBLIC 
        RapidJSON
    PRIVATE 
        OTTERUS_LOGGER 
        tinyfiledialogs
)
