find_package(PkgConfig REQUIRED)

pkg_check_modules(
    GSTREAMER
    REQUIRED
    IMPORTED_TARGET
    gstreamer-1.0
)

add_executable(streamscope_player
    src/main.cpp
    src/hls_manifest.cpp
    src/segment_scheduler.cpp
)

target_include_directories(
    streamscope_player
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    streamscope_player
    PRIVATE
        PkgConfig::GSTREAMER
)

target_compile_options(
    streamscope_player
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
