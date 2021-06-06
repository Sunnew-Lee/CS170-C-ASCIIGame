/*
    sunwoo.lee
    ASCII Game
    CS170
    Spring 2021
*/
#include "dp_console.h"
#include <stdio.h>
#include <stdlib.h>
#include "Strength.h"
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main(void)
{
    Strength game = { 0 };
    bool hide_cursor = true;
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    dpc_startup(hide_cursor);
    Read_results();

    Game_setup(&game);
    Map_setup(&game);
    Draw_map(&game);

    while (!(Get_Flag(&game.Myflag, Get_Isdone)))
    {
        Draw_timer(&game);

        if (Game_update(&game, dpc_get_key_async()) == true)
        {
            Draw_map(&game);
        }
        dpc_flush();
    }

    Write_results(&game);
    return 0;
}
