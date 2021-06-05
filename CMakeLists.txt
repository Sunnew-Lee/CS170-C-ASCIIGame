cmake_minimum_required(VERSION 3.10)

project(c-ascii-game-sunwoo-lee)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c89 -Wno-implicit-fallthrough)
endif()

add_executable(c-ascii-game-sunwoo-lee main.c dp_console.c Strength.c)