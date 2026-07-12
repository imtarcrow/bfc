cmake_minimum_required(VERSION 3.25)
project(bfc VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(bfc 
    ./src/main.c
    ./src/state.c
    ./src/parse.c
)

target_include_directories(bfc PRIVATE ./include/)
target_compile_options(bfc PRIVATE 
    $<$<CONFIG:Debug>:-g -Wall -Wextra -Wpedantic -fsanitize=address -fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O2> 
)

target_link_options(bfc PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
)
