find_package(PkgConfig REQUIRED)

pkg_check_modules(
    GSTREAMER
    REQUIRED
    IMPORTED_TARGET
    gstreamer-1.0
)

pkg_check_modules(
    GSTREAMER_APP
    REQUIRED
    IMPORTED_TARGET
    gstreamer-app-1.0
)

pkg_check_modules(
    LIBCURL
    REQUIRED
    IMPORTED_TARGET
    libcurl
)

add_executable(streamscope_player
    src/main.cpp
    src/hls_manifest.cpp
    src/segment_scheduler.cpp
    src/http_downloader.cpp
    src/playback_state.cpp
    src/buffer_model.cpp
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
        PkgConfig::GSTREAMER_APP
        PkgConfig::LIBCURL
)

target_compile_options(
    streamscope_player
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
