#pragma once
/*
    DigiPen Console Util Library
    Copyright (C) 2020, DigiPen Institute of Technology, All Rights Reserved
*/

#include <stdbool.h>

/*
    These are colors that you can use for printing characters.
*/
typedef enum
{
    dpc_BLACK,
    dpc_BLUE,
    dpc_GREEN,
    dpc_CYAN,
    dpc_RED,
    dpc_MAGENTA,
    dpc_BROWN,
    dpc_GREY,
    dpc_DARKGREY,
    dpc_LIGHTBLUE,
    dpc_LIGHTGREEN,
    dpc_LIGHTCYAN,
    dpc_LIGHTRED,
    dpc_LIGHTMAGENTA,
    dpc_YELLOW,
    dpc_WHITE
} dpcConsoleColors;

/*
    These are the colors you can use for the background of characters.
*/
typedef enum
{
    dpc_BACKGROUND_BLACK,
    dpc_BACKGROUND_BLUE,
    dpc_BACKGROUND_GREEN,
    dpc_BACKGROUND_CYAN,
    dpc_BACKGROUND_RED,
    dpc_BACKGROUND_MAGENTA,
    dpc_BACKGROUND_YELLOW,
    dpc_BACKGROUND_WHITE
} dpcConsoleBackgroundColors;


/*
    For keyboard buttons that are letter or numbers you can check against
    the corresponding character literal. Like 'a' for the a button.
    For other keys, used the following enum values.
*/
typedef enum
{
    dpc_KEY_ESCAPE = 0,
    dpc_KEY_ENTER  = 1,
    dpc_KEY_SPACE  = 32,

    dpc_KEY_INSERT = 2,
    dpc_KEY_HOME   = 3,
    dpc_KEY_PGUP   = 4,
    dpc_KEY_DELETE = 5,
    dpc_KEY_END    = 6,
    dpc_KEY_PGDOWN = 7,

    dpc_KEY_UP    = 14,
    dpc_KEY_DOWN  = 15,
    dpc_KEY_LEFT  = 16,
    dpc_KEY_RIGHT = 17,

    dpc_KEY_F1  = 18,
    dpc_KEY_F2  = 19,
    dpc_KEY_F3  = 20,
    dpc_KEY_F4  = 21,
    dpc_KEY_F5  = 22,
    dpc_KEY_F6  = 23,
    dpc_KEY_F7  = 24,
    dpc_KEY_F8  = 25,
    dpc_KEY_F9  = 26,
    dpc_KEY_F10 = 27,

    dpc_KEY_NUMDEL  = 30,
    dpc_KEY_NUMPAD0 = 31,
    dpc_KEY_NUMPAD1 = 127,
    dpc_KEY_NUMPAD2 = 128,
    dpc_KEY_NUMPAD3 = 129,
    dpc_KEY_NUMPAD4 = 130,
    dpc_KEY_NUMPAD5 = 131,
    dpc_KEY_NUMPAD6 = 132,
    dpc_KEY_NUMPAD7 = 133,
    dpc_KEY_NUMPAD8 = 134,
    dpc_KEY_NUMPAD9 = 135
} dpcKeyCodes;


/*
    Call this one time at the start of your program.
    If you wish to hide the console cursor then pass
    along true to the hide_cursor parameter.
*/
void dpc_startup(bool hide_cursor);

/*
    This returns the console height in number of characters
*/
int dpc_get_console_height(void);

/*
    This returns the console width in number of characters
*/
int dpc_get_console_width(void);

/*
    Resets the entire console to the space ' ' character
    and moves the cursor position to the very top left (0,0)
*/
void dpc_clear_console_screen(void);

/*
    Use this to position the cursor somewhere specific on the console.
    (0,0) is the very top left.
    You will see undefined behavior if you pass in a negative value or
    a position that is >= the console width/height
*/
void dpc_move_cursor(int x, int y);

/*
    Sets the color to use when print characters.
    After calling this function, all following print related functions
    (printf, putchar, etc) will output characters in the specified color.
*/
void dpc_set_font_color(dpcConsoleColors color);

/*
    Sets the background color for the next characters to be printed.
*/
void dpc_set_font_background_color(dpcConsoleBackgroundColors color);

/*
    This forces the console to visualize all print commands.
    By default, functions like printf have an internal cache and only
    print to the console when it's cache is full.

    We recommend that you call this at the end of your game loop to make
    sure you see any changes that you printed for that game loop iteration.
*/
void dpc_flush(void);

/*
    This will print the optional message (just pass NULL for no message) and
    then waits for any keyboard button to be pressed before returning from the
    function.
*/
void dpc_wait_for_anykey(const char* anykey_message);

/*
    This function can be used to check for a key press and it won't wait if the
    user didn't press anything.

    Returns -1 if nothing was pressed, otherwise returns a dpcKeyCodes value of
    the key that was pressed.
*/
int dpc_get_key_async(void);

/*
    Returns true or false, if they keyboard was pressed or not.
*/
bool dpc_keyboard_was_pressed(void);

/*
    This function will wait until the user presses a keyboard button before
    returning a dpcKeyCodes value of the key that was pressed.
*/
int dpc_get_key(void);
