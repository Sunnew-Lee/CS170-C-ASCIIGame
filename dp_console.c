/*
    DigiPen Console Util Library
    Copyright (C) 2020, DigiPen Institute of Technology, All Rights Reserved
*/
#include "dp_console.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN

#    define NOGDICAPMASKS     /* CC_*, LC_*, PC_*, CP_*, TC_*, RC_ */
#    define NOVIRTUALKEYCODES /* VK_* */
#    define NOWINMESSAGES     /* WM_*, EM_*, LB_*, CB_* */
#    define NOWINSTYLES       /* WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_* */
#    define NOSYSMETRICS      /* SM_* */
#    define NOMENUS           /* MF_* */
#    define NOICONS           /* IDI_* */
#    define NOKEYSTATES       /* MK_* */
#    define NOSYSCOMMANDS     /* SC_* */
#    define NORASTEROPS       /* Binary and Tertiary raster ops */
#    define NOSHOWWINDOW      /* SW_* */
#    define OEMRESOURCE       /* OEM Resource values */
#    define NOATOM            /* Atom Manager routines */
#    define NOCLIPBOARD       /* Clipboard routines */
#    define NOCOLOR           /* Screen colors */
#    define NOCTLMGR          /* Control and Dialog routines */
#    define NODRAWTEXT        /* DrawText() and DT_* */
#    define NOGDI             /* All GDI defines and routines */
#    define NOKERNEL          /* All KERNEL defines and routines */
#    define NOUSER            /* All USER defines and routines */
#    define NONLS             /* All NLS defines and routines */
#    define NOMB              /* MB_* and MessageBox() */
#    define NOMEMMGR          /* GMEM_*, LMEM_*, GHND, LHND, associated routines */
#    define NOMETAFILE        /* typedef METAFILEPICT */
#    define NOMINMAX          /* Macros min(a,b) and max(a,b) */
#    define NOMSG             /* typedef MSG and associated routines */
#    define NOOPENFILE        /* OpenFile(), OemToAnsi, AnsiToOem, and OF_* */
#    define NOSCROLL          /* SB_* and scrolling routines */
#    define NOSERVICE         /* All Service Controller routines, SERVICE_ equates, etc. */
#    define NOSOUND           /* Sound driver routines */
#    define NOTEXTMETRIC      /* typedef TEXTMETRIC and associated routines */
#    define NOWH              /* SetWindowsHook and WH_* */
#    define NOWINOFFSETS      /* GWL_*, GCL_*, associated routines */
#    define NOCOMM            /* COMM driver routines */
#    define NOKANJI           /* Kanji support stuff. */
#    define NOHELP            /* Help engine interface. */
#    define NOPROFILER        /* Profiler interface. */
#    define NODEFERWINDOWPOS  /* DeferWindowPos routines */
#    define NOMCX             /* Modem Configuration Extensions */
#    include <Windows.h>      /* for WinAPI and Sleep() */
#    define _NO_OLDNAMES      /* for MinGW compatibility */
#    include <conio.h>        /* for getch() and kbhit() */

static bool                       HidCursor = false;
static HANDLE                     StdOutHandle;
static CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
static WORD                       StartingColors;

static void dpc_set_cursor_visibility(bool visible)
{
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(StdOutHandle, &structCursorInfo);
    structCursorInfo.bVisible = (visible ? TRUE : FALSE);
    SetConsoleCursorInfo(StdOutHandle, &structCursorInfo);
}

static void dpc_shutdown(void)
{
    if (HidCursor)
        dpc_set_cursor_visibility(true);
    SetConsoleTextAttribute(StdOutHandle, StartingColors);
    dpc_clear_console_screen();
}

void dpc_startup(bool hide_cursor)
{
    StdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(StdOutHandle, &ConsoleInfo);
    StartingColors = ConsoleInfo.wAttributes;
    HidCursor      = hide_cursor;
    if (HidCursor)
        dpc_set_cursor_visibility(false);
    dpc_clear_console_screen();
    atexit(dpc_shutdown);
}

int dpc_get_console_height(void)
{
    GetConsoleScreenBufferInfo(StdOutHandle, &ConsoleInfo);
    return ConsoleInfo.srWindow.Bottom - ConsoleInfo.srWindow.Top + 1;
}

int dpc_get_console_width(void)
{
    GetConsoleScreenBufferInfo(StdOutHandle, &ConsoleInfo);
    return ConsoleInfo.srWindow.Right - ConsoleInfo.srWindow.Left + 1;
}

void dpc_clear_console_screen(void)
{
    const COORD coordScreen = {0, 0};
    DWORD       cCharsWritten;
    DWORD       dwConSize;

    GetConsoleScreenBufferInfo(StdOutHandle, &ConsoleInfo);
    dwConSize = (DWORD)(ConsoleInfo.dwSize.X * ConsoleInfo.dwSize.Y);

    FillConsoleOutputCharacter(StdOutHandle, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
    FillConsoleOutputAttribute(StdOutHandle, ConsoleInfo.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(StdOutHandle, coordScreen);
}

void dpc_move_cursor(int x, int y)
{
    COORD pos;
    pos.X = (short)x;
    pos.Y = (short)y;
    SetConsoleCursorPosition(StdOutHandle, pos);
}

void dpc_set_font_color(dpcConsoleColors color)
{
    GetConsoleScreenBufferInfo(StdOutHandle, &ConsoleInfo);
    SetConsoleTextAttribute(StdOutHandle, (WORD)((ConsoleInfo.wAttributes & 0xFFF0) | color));
}

void dpc_set_font_background_color(dpcConsoleBackgroundColors color)
{
    GetConsoleScreenBufferInfo(StdOutHandle, &ConsoleInfo);
    SetConsoleTextAttribute(StdOutHandle, (WORD)((ConsoleInfo.wAttributes & 0xFF0F) | ((color) << 4)));
}

void dpc_flush(void)
{
    FlushConsoleInputBuffer(StdOutHandle);
    if (HidCursor)
        dpc_set_cursor_visibility(false);
}

void dpc_wait_for_anykey(const char* anykey_message)
{
    if (anykey_message)
    {
        printf("%s", anykey_message);
    }
    _getch();
}

int dpc_get_key_async(void)
{
    if (_kbhit())
        return _getch();
    return -1;
}

bool dpc_keyboard_was_pressed(void)
{
    return _kbhit();
}

int dpc_get_key(void)
{
    int keycode = _getch();
    switch (keycode)
    {
        case 0:
        {
            int next_keycode;
            switch (next_keycode = _getch())
            {
                case 71: return dpc_KEY_NUMPAD7;
                case 72: return dpc_KEY_NUMPAD8;
                case 73: return dpc_KEY_NUMPAD9;
                case 75: return dpc_KEY_NUMPAD4;
                case 77: return dpc_KEY_NUMPAD6;
                case 79: return dpc_KEY_NUMPAD1;
                case 80: return dpc_KEY_NUMPAD2;
                case 81: return dpc_KEY_NUMPAD3;
                case 82: return dpc_KEY_NUMPAD0;
                case 83: return dpc_KEY_NUMDEL;
                default: return next_keycode - 59 + dpc_KEY_F1;
            }
        }
        case 224:
        {
            int next_keycode;
            switch (next_keycode = _getch())
            {
                case 71: return dpc_KEY_HOME;
                case 72: return dpc_KEY_UP;
                case 73: return dpc_KEY_PGUP;
                case 75: return dpc_KEY_LEFT;
                case 77: return dpc_KEY_RIGHT;
                case 79: return dpc_KEY_END;
                case 80: return dpc_KEY_DOWN;
                case 81: return dpc_KEY_PGDOWN;
                case 82: return dpc_KEY_INSERT;
                case 83: return dpc_KEY_DELETE;
                default: return next_keycode - 123 + dpc_KEY_F1;
            }
        }
        case 13: return dpc_KEY_ENTER;
        case 27: return dpc_KEY_ESCAPE;
        default: return keycode;
    }
}
#else

#    include <sys/ioctl.h>
#    include <sys/time.h>
#    include <sys/types.h>
#    include <termios.h>
#    include <unistd.h>


static const char* const ANSI_CLS                = "\033[2J\033[3J";
static const char* const ANSI_ATTRIBUTE_RESET    = "\033[0m";
static const char* const ANSI_CURSOR_HIDE        = "\033[?25l";
static const char* const ANSI_CURSOR_SHOW        = "\033[?25h";
static const char* const ANSI_CURSOR_HOME        = "\033[H";
static const char* const ANSI_BLACK              = "\033[22;30m";
static const char* const ANSI_RED                = "\033[22;31m";
static const char* const ANSI_GREEN              = "\033[22;32m";
static const char* const ANSI_BROWN              = "\033[22;33m";
static const char* const ANSI_BLUE               = "\033[22;34m";
static const char* const ANSI_MAGENTA            = "\033[22;35m";
static const char* const ANSI_CYAN               = "\033[22;36m";
static const char* const ANSI_GREY               = "\033[22;37m";
static const char* const ANSI_DARKGREY           = "\033[01;30m";
static const char* const ANSI_LIGHTRED           = "\033[01;31m";
static const char* const ANSI_LIGHTGREEN         = "\033[01;32m";
static const char* const ANSI_YELLOW             = "\033[01;33m";
static const char* const ANSI_LIGHTBLUE          = "\033[01;34m";
static const char* const ANSI_LIGHTMAGENTA       = "\033[01;35m";
static const char* const ANSI_LIGHTCYAN          = "\033[01;36m";
static const char* const ANSI_WHITE              = "\033[01;37m";
static const char* const ANSI_BACKGROUND_BLACK   = "\033[40m";
static const char* const ANSI_BACKGROUND_RED     = "\033[41m";
static const char* const ANSI_BACKGROUND_GREEN   = "\033[42m";
static const char* const ANSI_BACKGROUND_YELLOW  = "\033[43m";
static const char* const ANSI_BACKGROUND_BLUE    = "\033[44m";
static const char* const ANSI_BACKGROUND_MAGENTA = "\033[45m";
static const char* const ANSI_BACKGROUND_CYAN    = "\033[46m";
static const char* const ANSI_BACKGROUND_WHITE   = "\033[47m";

static const char* ToANSIColor(dpcConsoleColors color)
{
    switch (color)
    {
        case dpc_BLACK: return ANSI_BLACK;
        case dpc_BLUE: return ANSI_BLUE;
        case dpc_GREEN: return ANSI_GREEN;
        case dpc_CYAN: return ANSI_CYAN;
        case dpc_RED: return ANSI_RED;
        case dpc_MAGENTA: return ANSI_MAGENTA;
        case dpc_BROWN: return ANSI_BROWN;
        case dpc_GREY: return ANSI_GREY;
        case dpc_DARKGREY: return ANSI_DARKGREY;
        case dpc_LIGHTBLUE: return ANSI_LIGHTBLUE;
        case dpc_LIGHTGREEN: return ANSI_LIGHTGREEN;
        case dpc_LIGHTCYAN: return ANSI_LIGHTCYAN;
        case dpc_LIGHTRED: return ANSI_LIGHTRED;
        case dpc_LIGHTMAGENTA: return ANSI_LIGHTMAGENTA;
        case dpc_YELLOW: return ANSI_YELLOW;
        case dpc_WHITE: return ANSI_WHITE;
        default: return ANSI_BLACK;
    }
}

static const char* ToANSIBackgroundColor(dpcConsoleBackgroundColors color)
{
    switch (color)
    {
        case dpc_BACKGROUND_BLACK: return ANSI_BACKGROUND_BLACK;
        case dpc_BACKGROUND_BLUE: return ANSI_BACKGROUND_BLUE;
        case dpc_BACKGROUND_GREEN: return ANSI_BACKGROUND_GREEN;
        case dpc_BACKGROUND_CYAN: return ANSI_BACKGROUND_CYAN;
        case dpc_BACKGROUND_RED: return ANSI_BACKGROUND_RED;
        case dpc_BACKGROUND_MAGENTA: return ANSI_BACKGROUND_MAGENTA;
        case dpc_BACKGROUND_YELLOW: return ANSI_BACKGROUND_YELLOW;
        case dpc_BACKGROUND_WHITE: return ANSI_BACKGROUND_WHITE;
        default: return ANSI_BACKGROUND_BLACK;
    }
}

static bool HidCursor = false;

static void dpc_shutdown(void)
{
    if (HidCursor)
        printf("%s", ANSI_CURSOR_SHOW);
    printf("%s", ANSI_ATTRIBUTE_RESET);
    dpc_clear_console_screen();
}

void dpc_startup(bool hide_cursor)
{
    HidCursor = hide_cursor;
    if (HidCursor)
        printf("%s", ANSI_CURSOR_HIDE);
    dpc_clear_console_screen();
    atexit(dpc_shutdown);
}


int dpc_get_console_height(void)
{
#    ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    return ts.ts_lines;
#    elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    return ts.ws_row;
#    else
    return -1;
#    endif
}

int dpc_get_console_width(void)
{
#    ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    return ts.ts_cols;
#    elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    return ts.ws_col;
#    else
    return -1;
#    endif
}

void dpc_clear_console_screen(void)
{
    printf("%s", ANSI_CLS);
    printf("%s", ANSI_CURSOR_HOME);
}

void dpc_move_cursor(int x, int y)
{
    ++x;
    ++y;
    printf("\033[%d;%df", y, x);
}

void dpc_set_font_color(dpcConsoleColors color)
{
    printf("%s", ToANSIColor(color));
}

void dpc_set_font_background_color(dpcConsoleBackgroundColors color)
{
    printf("%s", ToANSIBackgroundColor(color));
}

void dpc_flush(void)
{
    fflush(stdout);
    if (HidCursor)
        printf("%s", ANSI_CURSOR_HIDE);
}

void dpc_wait_for_anykey(const char* anykey_message)
{
    if (anykey_message)
        printf("%s", anykey_message);
    dpc_get_key();
}

static int dpc_kbhit(void)
{
    static struct termios oldt, newt;
    int                   cnt = 0;
    struct timeval        tv;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~((unsigned)(ICANON | ECHO));
    newt.c_iflag     = 0; /* input mode */
    newt.c_oflag     = 0; /* output mode */
    newt.c_cc[VMIN]  = 1; /* minimum time to wait */
    newt.c_cc[VTIME] = 1; /* minimum characters to wait for */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ioctl(0, FIONREAD, &cnt); /* Read count */
    tv.tv_sec  = 0;
    tv.tv_usec = 100;
    select(STDIN_FILENO + 1, NULL, NULL, NULL, &tv); /* A small time delay */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return cnt; /* Return number of characters */
}

static int dpc_getch(void)
{
    struct termios oldt, newt;
    int            ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~((unsigned)(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int dpc_get_key_async(void)
{
    if (dpc_kbhit())
        return dpc_getch();
    return -1;
}

bool dpc_keyboard_was_pressed(void)
{
    return dpc_kbhit();
}

int dpc_get_key(void)
{
    int keycode = dpc_getch();
    switch (keycode)
    {
        case 0:
        {
            int next_keycode = dpc_getch();
            switch (next_keycode)
            {
                case 71: return dpc_KEY_NUMPAD7;
                case 72: return dpc_KEY_NUMPAD8;
                case 73: return dpc_KEY_NUMPAD9;
                case 75: return dpc_KEY_NUMPAD4;
                case 77: return dpc_KEY_NUMPAD6;
                case 79: return dpc_KEY_NUMPAD1;
                case 80: return dpc_KEY_NUMPAD2;
                case 81: return dpc_KEY_NUMPAD3;
                case 82: return dpc_KEY_NUMPAD0;
                case 83: return dpc_KEY_NUMDEL;
                default: return next_keycode - 59 + dpc_KEY_F1;
            }
        }
        break;
        case 13: return dpc_KEY_ENTER;
        case 224:
        {
            int next_keycode = dpc_getch();
            switch (next_keycode)
            {
                case 71: return dpc_KEY_HOME;
                case 72: return dpc_KEY_UP;
                case 73: return dpc_KEY_PGUP;
                case 75: return dpc_KEY_LEFT;
                case 77: return dpc_KEY_RIGHT;
                case 79: return dpc_KEY_END;
                case 80: return dpc_KEY_DOWN;
                case 81: return dpc_KEY_PGDOWN;
                case 82: return dpc_KEY_INSERT;
                case 83: return dpc_KEY_DELETE;
                default: return next_keycode - 123 + dpc_KEY_F1;
            }
        }
        break;
        case 155:
        case 27:
        {
            int next_keycode = dpc_getch();
            if (next_keycode == '[')
            {
                next_keycode = dpc_getch();
                switch (next_keycode)
                {
                    case 'A': return dpc_KEY_UP;
                    case 'B': return dpc_KEY_DOWN;
                    case 'C': return dpc_KEY_RIGHT;
                    case 'D': return dpc_KEY_LEFT;
                }
            }
            else
                return dpc_KEY_ESCAPE;
        }

        default: return keycode;
    }
}
#endif