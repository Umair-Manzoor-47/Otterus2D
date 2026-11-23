add_library(OTTERUS_FILESYSTEM STATIC
    # Dialogs
    include/OtterusFileSystem/Dialogs/FileDialog.h
    src/FileDialog.cpp

    # Serializers
    include/OtterusFileSystem/Serializers/JSONSerializer.h
    include/OtterusFileSystem/Serializers/JSONSerializer.inl
    src/JSONSerializer.cpp
)

target_include_directories(OTTERUS_FILESYSTEM PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(OTTERUS_FILESYSTEM
    PRIVATE
        OTTERUS_LOGGER
        tinyfiledialogs
        RapidJSON
)
