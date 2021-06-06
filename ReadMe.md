# ASCII Game in C Assignment
## 1. Your name, project name, course name, term
- Name: hyun.kang / sunwoo.lee
- Project Name: ASCII Game in C
- Course Name: CS170
- Term: Spring 2021

## 2. How to build with make
- Type 'bash' on the folder path where all source files are at.
- Type 'make' on the Ubuntu bash shell.

## 3. How to build with Cmake
- Make a empty folder on the folder where all source files are at.
- Windows
    - Type 'cmd' on the file path which you created.
    - Type 'cmake \.\.' on the command prompt.
    - Type 'cmake --build .' on the command prompt.
- Linux
    - Type 'bash' on the file path which you created.
    - Type 'cmake \.\.' on the command prompt.
    - Type 'make' on the command prompt.

## 4. Introduction of your game
- Control the player to push the bolders in to the pit and go to the next level!
- More faster and less movements will have impact on score!

## 5. How to interact with the game
- Contol the player with keyboard 'W', 'S', 'A', 'D'.
- Press keyboard 'R' to restart the level.
- Press keyboard 'Esc' to quit the game.

## 6. Section describing something you're proud of about the project
- I have learned C language before I went to military service, so it was a bit tough to handle C.
- Also, the differences between windows and linux drove me nuts, but I'm proud that we made the game without any warnings.

## 7. Section describing anything incomplete about the project
- Some level design and more levels.

## 8. Section to Provide file name and line numbers for all code related requirements
- [core] Have custom command line arguments
    - CMakeLists.txt - line 13
- [core] Make good use of arrays
    - Strength.c - line 188 
- [core] Use Bitwise operations to create a some kind of configuration variable where you extract values.
    - Strength.c - line 28, 30, 32, 34 
- [core] When applicable, use bitwise operations to replace Arithmetic Operations
    - Strength.c - line 257, 262, 267, 272 
- [core] Make good use of struct
    - Strength.h - line 24 
- [core] Make good use of malloc()/free()
    - Strength.c - line 100, 101 
- [core] Make good use of typedef to simplify your types
    - Strength.h - line 15, 24, 37, 38 
- [core] Make good use of #define for number literals
    - Strength.c - line 9, 10, 11, 12, 13 
- [core] Make good use of C File IO
    - Strength.c - line 57, 160 
- [core] Make good use of Function Pointers
    - Strength.c - line 24, 26 
- Kept global variables to a minimum
    - main.c - line 18 
- Reused functions as much as possible.
    - Strength.c - line 149, 250, 322
