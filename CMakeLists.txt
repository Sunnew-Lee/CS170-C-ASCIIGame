cmake_minimum_required(VERSION 3.10)

project(Strength)

set(CMAKE_C_STANDARD_REQUIRED TRUE)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c89 -Wno-implicit-fallthrough)
endif()

add_executable(Strength main.c dp_console.c Strength.c)