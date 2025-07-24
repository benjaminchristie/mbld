cmake_minimum_required(VERSION 3.25)

project(mbld)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(simple_mbld
    src/simple_mbld_main.c
    src/simple_mbld.c
)
target_include_directories(simple_mbld PUBLIC include/simple_mbld)
target_compile_options(simple_mbld PUBLIC 
    -Wall -Werror -Wpedantic 
    -O3 -fomit-frame-pointer -march=native
)

add_executable(raytracer src/raytracer.c)
target_link_libraries(raytracer PUBLIC m)