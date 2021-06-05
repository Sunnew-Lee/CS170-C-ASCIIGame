/*
    sunwoo.lee
    ASCII Game
    CS170
    Spring 2021
*/


#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#define MAX_TEXT_SIZE 100
#define MAX_LEVEL 10
#define MAX_TIME 100
#define ESCAPE 27
#define LINUX_TIME 187500
#include "Strength.h"
#include "dp_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char IsDone=1<<0;
char IsCleared=1<<1;

void Set_Flag(Strength* game, FuncPtr func)
{
    func(game);
}

void Set_Isdone(Strength* game)
{
    game->Myflag |= IsDone;
}

void Set_Iscleared(Strength* game)
{
    game->Myflag |= IsCleared;
}

void Game_intro()
{
    FILE* fp;
    char* a = malloc(sizeof(char) * MAX_TEXT_SIZE);
    dpc_set_font_background_color(dpc_BACKGROUND_BLACK);

    dpc_set_font_color(dpc_GREEN);
    printf("Title:\t\tGame of Strength\n");
    printf("How to play:\tUse WASD to move, R to reset level, ESC to quit.\n");

    dpc_set_font_color(dpc_WHITE);
    printf("Player:\t\tP\n");
    dpc_set_font_color(dpc_DARKGREY);
    printf("Wall:\t\t#\n");
    dpc_set_font_color(dpc_LIGHTGREEN);
    printf("Next Level:\tQ\n");
    dpc_set_font_color(dpc_BROWN);
    printf("Rock:\t\t@\n");
    dpc_set_font_color(dpc_LIGHTRED);
    printf("Pit:\t\tO\n");

    fp = fopen("assets/Results.txt", "rt");

    if (a == NULL)
    {
        return;
    }

    if (fp)
    {
        dpc_set_font_color(dpc_LIGHTGREEN);
        printf("================================================================\n");
        dpc_set_font_color(dpc_WHITE);
        printf("Last Result\n");
        while (!feof(fp) && fgets(a, MAX_TEXT_SIZE, fp))
        {
            printf("%s",a);
        }
        fclose(fp);
    }

    free(a);
    dpc_set_font_color(dpc_LIGHTGREEN);
    dpc_wait_for_anykey("Press any key to start.\n");
}

void Game_setup(Strength* game) { game->Score = malloc(sizeof(int) * MAX_LEVEL); }

void Map_setup(Strength* game)
{
    int i;
    char* c1 = malloc(sizeof(char) * MAX_TEXT_SIZE);
    char* c2 = malloc(sizeof(char) * MAX_TEXT_SIZE);

    if (game->CurrentLevel > MAX_LEVEL)
    {
        game->CurrentLevel = MAX_LEVEL;
        Set_Flag(game, Set_Isdone);
        Set_Flag(game, Set_Iscleared);
        return;
    }

    if (game->Map != 0)
    {
        for (i = 0; i < game->Height; i++)
        {
            free(game->Map[i]);
        }
        free(game->Map);
    }

    if (c1 == NULL)
    {
        printf("There is No File. \n");
        return;
    }
    if (c2 == NULL)
    {
        printf("There is No File. \n");
        return;
    }
    game->File_Name = malloc(sizeof(char) * MAX_TEXT_SIZE);
    if (game->File_Name == NULL)
    {
        printf("There is No File. \n");
        return;
    }

    strcpy(game->File_Name, "assets/Map");
    sprintf(c1, "%d", game->CurrentLevel);



    strcpy(c2, ".txt");

    strcat(game->File_Name, c1);
    free(c1);
    strcat(game->File_Name, c2);
    free(c2);

    Create_map(game);
    Map_reset(game);
}

void Create_map(Strength* game)
{
    int i;
    FILE* fp;
    char* a = malloc(sizeof(char) * MAX_TEXT_SIZE);

    fp = fopen(game->File_Name, "rt");
    
    if (!fp)
    {
        dpc_clear_console_screen();
        printf("No Such File: ");
        printf("%s\n", game->File_Name);
    }
    else
    {
        while (!feof(fp))
        {
            a=fgets(a, MAX_TEXT_SIZE, fp);

            if (atoi(a) != 0)
            {
                game->Width = atoi(a);
                a=fgets(a, MAX_TEXT_SIZE, fp);
                a=fgets(a, MAX_TEXT_SIZE, fp);
                game->Height = atoi(a);
            }
        }
    }
    if (game->Width <= 0 || game->Height <= 0) {
        printf("Illegal number of rows or columns!\n");
        Set_Flag(game, Set_Isdone);
        return;
    }
    game->Map = (Tiles**)malloc(sizeof(Tiles*) * (long unsigned int)game->Height); 
    if (game->Map == NULL)
    {
        Set_Flag(game, Set_Isdone);
        return;
    }
    for (i = 0; i < game->Height; i++) 
    {
        game->Map[i] = (Tiles*)malloc(sizeof(Tiles) * (long unsigned int)game->Width);
        if (game->Map[i] == NULL)
        {
            Set_Flag(game, Set_Isdone);
            return;
        }
    }

    free(a);
    fclose(fp);
}

void Map_reset(Strength* game)
{
    FILE* fp;
    int i;
    int j;
    dpc_clear_console_screen();

    fp = fopen(game->File_Name, "rt");

    for (i = 0; i < game->Height; i++)
    {
        for (j = 0; j < game->Width; j++)
        {
            switch (fgetc(fp))
            {
            case 'P':game->Map[i][j] = ROAD; game->PlayerX = j; game->PlayerY = i; break;
            case ' ':game->Map[i][j] = ROAD; break;
            case '#':game->Map[i][j] = WALL; break;
            case '@':game->Map[i][j] = ROCK; break;
            case 'O':game->Map[i][j] = PIT; break;
            case 'Q':game->Map[i][j] = NEXT_LEVEL; break;
            default:j--; break;
            }
        }
    }
    fclose(fp);
    game->MovesCount = 0;
    game->Start = clock();

}


bool Game_update(Strength* game, int key_code)
{
    bool did_move = false;
    int  new_x = game->PlayerX;
    int  new_y = game->PlayerY;

    if (key_code == ESCAPE)
    {
        Set_Flag(game, Set_Isdone);
        return false;
    }
    else if (key_code == 'R' || key_code == ('R' ^ (1 << 5)))
    {
        Map_reset(game);
        return true;
    }

    switch (key_code)
    {
    case 'A':
    case ('A' ^ (1 << 5)):
        --new_x;
        did_move = true;
        break;
    case 'D':
    case ('D' ^ (1 << 5)):
        ++new_x;
        did_move = true;
        break;
    case 'W':
    case ('W' ^ (1 << 5)):
        --new_y;
        did_move = true;
        break;
    case 'S':
    case ('S' ^ (1 << 5)):
        ++new_y;
        did_move = true;
        break;
    default: break;
    }

    if (!did_move || game->Map[new_y][new_x] == WALL || game->Map[new_y][new_x] == PIT)
    {
        return false;
    }

    ++game->MovesCount;

    switch (game->Map[new_y][new_x])
    {
    case ROCK:
        switch (new_x - game->PlayerX)
        {
        case 1:
            if (game->Map[new_y][new_x + 1] == ROAD) { game->Map[new_y][new_x + 1] = ROCK; }
            else if (game->Map[new_y][new_x + 1] == PIT) { game->Map[new_y][new_x + 1] = ROAD; }
            else { return false; }
            break;
        case -1:
            if (game->Map[new_y][new_x - 1] == ROAD) { game->Map[new_y][new_x - 1] = ROCK; }
            else if (game->Map[new_y][new_x - 1] == PIT) { game->Map[new_y][new_x - 1] = ROAD; }
            else { return false; }
            break;
        default:break;
        }

        switch (new_y - game->PlayerY)
        {
        case 1:
            if (game->Map[new_y + 1][new_x] == ROAD) { game->Map[new_y + 1][new_x] = ROCK; }
            else if (game->Map[new_y + 1][new_x] == PIT) { game->Map[new_y + 1][new_x] = ROAD; }
            else { return false; }
            break;
        case -1:
            if (game->Map[new_y - 1][new_x] == ROAD) { game->Map[new_y - 1][new_x] = ROCK; }
            else if (game->Map[new_y - 1][new_x] == PIT) { game->Map[new_y - 1][new_x] = ROAD; }
            else { return false; }
            break;
        default:break;
        }
        game->Map[new_y][new_x] = ROAD;
        game->PlayerX = new_x;
        game->PlayerY = new_y;
        break;
    case NEXT_LEVEL: Go_to_next_level(game);
        Map_setup(game); if ((game->Myflag & IsDone)) { return false; }
        break;
    default:
        game->PlayerX = new_x;
        game->PlayerY = new_y;
        break;
    }

    return true;
}

void Go_to_next_level(Strength* game)
{
    free(game->File_Name);
    *(game->Score + game->CurrentLevel - 1) = (game->MovesCount << 2) * MAX_LEVEL;
    game->CurrentLevel++;
}

void Draw_map(Strength* game)
{
    int i;
    int j;
    dpc_move_cursor(0, game->Height+1);
    dpc_set_font_color(dpc_LIGHTMAGENTA);
    printf("Moves: %d\n", game->MovesCount);
    dpc_set_font_color(dpc_GREEN);
    printf("Level: %d\n", game->CurrentLevel);
    dpc_move_cursor(0, 0);

    for (i = 0; i < game->Height; i++)
    {
        for (j = 0; j < game->Width; j++)
        {

            switch (game->Map[i][j])
            {
            case ROAD:
                dpc_set_font_color(dpc_BLUE);
                putchar('.');
                break;
            case WALL:
                dpc_set_font_color(dpc_BLACK + game->CurrentLevel);
                putchar('#');
                break;
            case ROCK:
                dpc_set_font_color(dpc_BROWN);
                putchar('@');
                break;
            case PIT:
                dpc_set_font_color(dpc_LIGHTRED);
                putchar('O');
                break;
            case NEXT_LEVEL:
                dpc_set_font_color(dpc_LIGHTGREEN);
                putchar('Q');
                break;
            default:;
            }
        }
        putchar('\n');
    }
    dpc_move_cursor(game->PlayerX, game->PlayerY);
    dpc_set_font_color(dpc_WHITE);
    putchar('P');
}

void Draw_timer(Strength* game)
{
#ifdef _WIN32
    dpc_move_cursor(0, game->Height);
    dpc_set_font_color(dpc_LIGHTRED);
    printf("Timer: %02lld", MAX_TIME-1 - (clock() - game->Start) / CLOCKS_PER_SEC);
    if ((clock() - game->Start) / CLOCKS_PER_SEC > MAX_TIME - 1)
    {
        Set_Flag(game, Set_Isdone);
    }
#else
    dpc_move_cursor(0, game->Height);
    dpc_set_font_color(dpc_LIGHTRED);
    printf("Timer: %02ld", MAX_TIME - 1 - (clock() - game->Start) / LINUX_TIME);
    if ((clock() - game->Start) / LINUX_TIME > MAX_TIME - 1)
    {
        Set_Flag(game, Set_Isdone);
    }
#endif
    dpc_move_cursor(0, 0);
}

void Write_results(Strength* game)
{
    FILE* fp;
    int i;
    char* c = malloc(sizeof(char) * MAX_TEXT_SIZE);

    dpc_clear_console_screen();
    dpc_set_font_color(dpc_YELLOW);

    for (i = 0; i < game->Height; i++)
    {
        free(game->Map[i]);
    }
    free(game->Map);

    fp = fopen("assets/Results.txt", "wt");

    printf("Top Level: %d\n", game->CurrentLevel);
    fputs("Top Level: ", fp);
    sprintf(c, "%d", game->CurrentLevel);

    fputs(c, fp);
    fputs("\n", fp);

    if ((game->Myflag & IsCleared))
    {
        printf("Game Cleared!\n");
        fputs("Game Cleared!\n", fp);

        for (i = 0; i < game->CurrentLevel; i++)
        {
            dpc_set_font_color(dpc_BLACK + i + 1);
            printf("%d Level Score: %d\n", i + 1, *(game->Score + i));
            sprintf(c, "%d", i+1);

            fputs(c, fp);
            fputs(" Level Score: ", fp);
            sprintf(c, "%d", *(game->Score + i));
 
            fputs(c, fp);
            fputs("\n", fp);
        }
    }
    else
    {
        for (i = 0; i < game->CurrentLevel - 1; i++)
        {
            dpc_set_font_color(dpc_BLACK + i + 1);
            printf("%d Level Score: %d\n", i + 1, *(game->Score + i));
            sprintf(c, "%d", i + 1);

            fputs(c, fp);
            fputs(" Level Score: ", fp);
            sprintf(c, "%d", *(game->Score + i));

            fputs(c, fp);
            fputs("\n", fp);
        }
        free(game->File_Name);
    }
    free(c);
    free(game->Score);
    fclose(fp);
    dpc_set_font_color(dpc_WHITE);
    dpc_wait_for_anykey("Hit any key to close.\n");
}