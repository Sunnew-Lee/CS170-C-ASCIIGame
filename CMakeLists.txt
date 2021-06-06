cmake_minimum_required(VERSION 3.10)

project(Strength)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c89 -Wno-implicit-fallthrough)
endif()

add_executable(Strength main.c dp_console.c Strength.c)

add_custom_command(
    TARGET Strength POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
"${CMAKE_CURRENT_SOURCE_DIR}/assets"
"${CMAKE_BINARY_DIR}/$<CONFIG>/assets"
)
