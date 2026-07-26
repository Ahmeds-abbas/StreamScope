add_executable(streamscope_player
    src/main.cpp
)

target_compile_options(streamscope_player PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)