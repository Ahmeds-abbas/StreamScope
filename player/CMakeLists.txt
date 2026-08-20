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
    src/telemetry_writer.cpp
    src/throughput_estimator.cpp
    src/abr_selector.cpp
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

find_package(GTest CONFIG REQUIRED)

add_executable(streamscope_tests
    tests/buffer_model_test.cpp
    tests/playback_state_test.cpp
    tests/telemetry_writer_test.cpp
    tests/throughput_estimator_test.cpp
    tests/abr_selector_test.cpp
    src/buffer_model.cpp
    src/playback_state.cpp
    src/telemetry_writer.cpp
    src/throughput_estimator.cpp
    src/abr_selector.cpp
)

target_include_directories(streamscope_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(streamscope_tests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(streamscope_tests)
