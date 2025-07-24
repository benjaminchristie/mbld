cmake_minimum_required(VERSION 3.25)

project(mbld)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VERSION)

add_executable(mbld_memo
    src/mbld/main.c
    src/mbld/mbld.c
)
target_include_directories(mbld_memo PUBLIC include)
target_compile_options(mbld_memo PUBLIC -Wall -Werror -Wpedantic -O3 -fomit-frame-pointer -march=native)

add_executable(virtual_cube
    src/virtual/cube.c
)
target_include_directories(virtual_cube PUBLIC include)
target_compile_options(virtual_cube PUBLIC -Wall -Werror -Wpedantic -O3 -fomit-frame-pointer -march=native)
set_target_properties(virtual_cube PROPERTIES C_STANDARD 23)
set_target_properties(virtual_cube PROPERTIES C_STANDARD_REQUIRED ON)