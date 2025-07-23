cmake_minimum_required(VERSION 3.25)

project(mbld)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main
    src/main.c
    src/mbld.c
)
target_include_directories(main PUBLIC include)
target_compile_options(main PUBLIC -Wall -Werror -Wpedantic -O3 -fomit-frame-pointer -march=native)