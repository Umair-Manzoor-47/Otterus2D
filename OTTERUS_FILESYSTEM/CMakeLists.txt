add_library(OTTERUS_FILESYSTEM
        "include/OtterusFileSystem/Dialogs/FileDialog.h"
        "src/FileDialog.cpp"
)

target_include_directories(
        OTTERUS_FILESYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(OTTERUS_FILESYSTEM PRIVATE OTTERUS_LOGGER tinyfiledialogs)

target_compile_options(
        OTTERUS_FILESYSTEM PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>
)