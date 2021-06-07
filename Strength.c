/*
 * hyun.kang / sunwoo.lee
 * ASCII Game in C Assignment
 * CS170
 * Spring 2021
 */
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#define MAX_TEXT_SIZE 100
#define MAX_LEVEL 10
#define MAX_TIME 100
#define ESCAPE 27
#define LINUX_TIME 187500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Strength.h"
#include "dp_console.h"

char IsDone=1<<0;
char IsCleared=1<<1;

void Set_Flag(char* myflag, VoidFuncPtr func) { func(myflag); }

bool Get_Flag(char* myflag, BoolFuncPtr func) { return func(myflag); }

void Set_Isdone(char* myflag) { *myflag |= IsDone; }

void Set_Iscleared(char* myflag) { *myflag |= IsCleared; }

bool Get_Isdone(char* myflag) { return (*myflag & IsDone); }

bool Get_Iscleared(char* myflag) { return (*myflag & IsCleared); }

void Read_results(int argc, char* argv[])
{
    FILE* fp;
    int i;
    char* container = malloc(sizeof(char) * MAX_TEXT_SIZE);
    dpc_set_font_background_color(dpc_BACKGROUND_BLACK);

    dpc_set_font_color(dpc_GREEN);
    printf("Player Name:\t");
    for( i = 1; i < argc; i++ )
    {
        printf("%s\t",argv[i]);
    }
    printf("\n");

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

    if (container == NULL)
    {
        return;
    }

    if (fp)
    {
        dpc_set_font_color(dpc_LIGHTGREEN);
        printf("================================================================\n");
        dpc_set_font_color(dpc_WHITE);
        printf("Last Result\n");
        while (!feof(fp) && fgets(container, MAX_TEXT_SIZE, fp))
        {
            printf("%s",container);
        }
        fclose(fp);
    }

    free(container);
    dpc_set_font_color(dpc_LIGHTGREEN);
    dpc_wait_for_anykey("Press any key to start.\n");
}

void Game_setup(Strength* game)
{
    game->Player_X = 0;
    game->Player_Y = 0;
    game->MovesCount = 0;
    game->Width = 0;
    game->Height = 0;
    game->CurrentLevel = 1;
    game->Score = malloc(sizeof(int) * MAX_LEVEL);
    game->Map = NULL;
    game->Start = 0;
    game->Myflag = 0;
    game->File_Name = NULL;
}

void Map_setup(Strength* game)
{
    int i;
    char* file_middle = malloc(sizeof(char) * MAX_TEXT_SIZE);
    char* file_last = malloc(sizeof(char) * MAX_TEXT_SIZE);

    if (game->CurrentLevel > MAX_LEVEL)
    {
        game->CurrentLevel = MAX_LEVEL;
        Set_Flag(&game->Myflag, Set_Isdone);
        Set_Flag(&game->Myflag, Set_Iscleared);
        free(file_middle);
        free(file_last);
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

    if (file_middle == NULL)
    {
        printf("There is No File. \n");
        return;
    }
    if (file_last == NULL)
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
    sprintf(file_middle, "%d", game->CurrentLevel);
    strcpy(file_last, ".txt");

    strcat(game->File_Name, file_middle);
    free(file_middle);
    strcat(game->File_Name, file_last);
    free(file_last);

    Create_map(game);
    Map_reset(game);
    game->Start = clock();
    game->MovesCount = 0;
}

void Create_map(Strength* game)
{
    int i;
    FILE* fp;
    char* container = malloc(sizeof(char) * MAX_TEXT_SIZE);

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
            container=fgets(container, MAX_TEXT_SIZE, fp);

            if (atoi(container) != 0)
            {
                game->Width = atoi(container);
                container=fgets(container, MAX_TEXT_SIZE, fp);
                container=fgets(container, MAX_TEXT_SIZE, fp);
                game->Height = atoi(container);
            }
        }
    }
    if (game->Width <= 0 || game->Height <= 0) {
        printf("Illegal number of rows or columns!\n");
        Set_Flag(&game->Myflag, Set_Isdone);
        return;
    }
    game->Map = (Tiles**)malloc(sizeof(Tiles*) * (long unsigned int)game->Height); 
    if (game->Map == NULL)
    {
        Set_Flag(&game->Myflag, Set_Isdone);
        return;
    }
    for (i = 0; i < game->Height; i++) 
    {
        game->Map[i] = (Tiles*)malloc(sizeof(Tiles) * (long unsigned int)game->Width);
        if (game->Map[i] == NULL)
        {
            Set_Flag(&game->Myflag, Set_Isdone);
            return;
        }
    }

    free(container);
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
            case 'P':game->Map[i][j] = ROAD; game->Player_X = j; game->Player_Y = i; break;
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
}


bool Game_update(Strength* game, int key)
{
    bool did_move = false;
    int  new_pos_X = game->Player_X;
    int  new_pos_Y = game->Player_Y;

    if (key == ESCAPE)
    {
        Set_Flag(&game->Myflag, Set_Isdone);
        return false;
    }
    else if (key == 'R' || key == ('R' ^ (1 << 5)))
    {
        Map_reset(game);
        return true;
    }

    switch (key)
    {
    case 'A':
    case ('A' ^ (1 << 5)):
        --new_pos_X;
        did_move = true;
        break;
    case 'D':
    case ('D' ^ (1 << 5)):
        ++new_pos_X;
        did_move = true;
        break;
    case 'W':
    case ('W' ^ (1 << 5)):
        --new_pos_Y;
        did_move = true;
        break;
    case 'S':
    case ('S' ^ (1 << 5)):
        ++new_pos_Y;
        did_move = true;
        break;
    default: return false;
    }

    if (!did_move || game->Map[new_pos_Y][new_pos_X] == WALL || game->Map[new_pos_Y][new_pos_X] == PIT)
    {
        return false;
    }

    ++game->MovesCount;

    switch (game->Map[new_pos_Y][new_pos_X])
    {
    case ROCK:
        switch (key)
        {
        case 'A':
        case ('A' ^ (1 << 5)):
            if (game->Map[new_pos_Y][new_pos_X - 1] == ROAD) { game->Map[new_pos_Y][new_pos_X - 1] = ROCK; }
            else if (game->Map[new_pos_Y][new_pos_X - 1] == PIT) { game->Map[new_pos_Y][new_pos_X - 1] = ROAD; }
            else { return false; }
            break;
        case 'D':
        case ('D' ^ (1 << 5)):
            if (game->Map[new_pos_Y][new_pos_X + 1] == ROAD) { game->Map[new_pos_Y][new_pos_X + 1] = ROCK; }
            else if (game->Map[new_pos_Y][new_pos_X + 1] == PIT) { game->Map[new_pos_Y][new_pos_X + 1] = ROAD; }
            else { return false; }
            break;
        case 'W':
        case ('W' ^ (1 << 5)):
            if (game->Map[new_pos_Y - 1][new_pos_X] == ROAD) { game->Map[new_pos_Y - 1][new_pos_X] = ROCK; }
            else if (game->Map[new_pos_Y - 1][new_pos_X] == PIT) { game->Map[new_pos_Y - 1][new_pos_X] = ROAD; }
            else { return false; }
            break;
        case 'S':
        case ('S' ^ (1 << 5)):
            if (game->Map[new_pos_Y + 1][new_pos_X] == ROAD) { game->Map[new_pos_Y + 1][new_pos_X] = ROCK; }
            else if (game->Map[new_pos_Y + 1][new_pos_X] == PIT) { game->Map[new_pos_Y + 1][new_pos_X] = ROAD; }
            else { return false; }
            break;
        default: break;
        }
        game->Map[new_pos_Y][new_pos_X] = ROAD;
        game->Player_X = new_pos_X;
        game->Player_Y = new_pos_Y;
        break;
    case NEXT_LEVEL: Go_to_next_level(game);
        Map_setup(game); if ((Get_Flag(&game->Myflag, Get_Isdone))) { return false; }
        break;
    default:
        game->Player_X = new_pos_X;
        game->Player_Y = new_pos_Y;
        break;
    }
    return true;
}

void Go_to_next_level(Strength* game)
{
#ifdef _WIN32
    int passed_time = (MAX_TIME - 1) - (int)((clock() - game->Start) / CLOCKS_PER_SEC);

#else
    int passed_time = (MAX_TIME - 1) - (int)((clock() - game->Start) / LINUX_TIME);

#endif
    * (game->Score + game->CurrentLevel - 1) = ((passed_time - (game->MovesCount / ((passed_time / MAX_LEVEL) + 1))) <= 0) ? 0 : (passed_time - (game->MovesCount / ((passed_time / MAX_LEVEL) + 1))) * MAX_LEVEL;
    free(game->File_Name);
    game->CurrentLevel++;
}

void Draw_map(Strength* game)
{
    int i;
    int j;
    dpc_move_cursor(0, game->Height + 1);
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
    dpc_move_cursor(game->Player_X, game->Player_Y);
    dpc_set_font_color(dpc_WHITE);
    putchar('P');
}

void Draw_timer(Strength* game)
{
#ifdef _WIN32
    dpc_move_cursor(0, game->Height);
    dpc_set_font_color(dpc_LIGHTRED);
    printf("Timer: %02lld", MAX_TIME - 1 - (clock() - game->Start) / CLOCKS_PER_SEC);
    if ((clock() - game->Start) / CLOCKS_PER_SEC > MAX_TIME - 1)
    {
        Set_Flag(&game->Myflag, Set_Isdone);
    }
#else
    dpc_move_cursor(0, game->Height);
    dpc_set_font_color(dpc_LIGHTRED);
    printf("Timer: %02ld", MAX_TIME - 1 - (clock() - game->Start) / LINUX_TIME);
    if ((clock() - game->Start) / LINUX_TIME > MAX_TIME - 1)
    {
        Set_Flag(&game->Myflag, Set_Isdone);
    }
#endif
    dpc_move_cursor(0, 0);
}

void Write_results(int argc, char* argv[],Strength* game)
{
    FILE* fp;
    int i;
    int flag=(!Get_Flag(&game->Myflag, Get_Iscleared));
    char* container = malloc(sizeof(char) * MAX_TEXT_SIZE);

    dpc_clear_console_screen();
    dpc_set_font_color(dpc_YELLOW);

    for (i = 0; i < game->Height; i++)
    {
        free(game->Map[i]);
    }
    free(game->Map);

    fp = fopen("assets/Results.txt", "wt");

    fputs("Player Name: ", fp);
    for( i = 1; i < argc; i++ )
    {
        fputs(argv[i],fp);
        fputs(" ",fp);
    }
    fputs("\n",fp);

    printf("Top Level: %d\n", game->CurrentLevel);
    fputs("Top Level: ", fp);
    sprintf(container, "%d", game->CurrentLevel);

    fputs(container, fp);
    fputs("\n", fp);

    if ((Get_Flag(&game->Myflag, Get_Iscleared)))
    {
        printf("Game Cleared!\n");
        fputs("Game Cleared!\n", fp);
    }
    else
    {
        free(game->File_Name);
    }

    for (i = 0; i < game->CurrentLevel - flag; i++)
    {
        dpc_set_font_color(dpc_BLACK + i + 1);
        printf("%d Level Score: %d\n", i + 1, *(game->Score + i));
        sprintf(container, "%d", i + 1);

        fputs(container, fp);
        fputs(" Level Score: ", fp);
        sprintf(container, "%d", *(game->Score + i));

        fputs(container, fp);
        fputs("\n", fp);
    }

    free(container);
    free(game->Score);
    fclose(fp);
    dpc_set_font_color(dpc_WHITE);
    dpc_wait_for_anykey("Hit any key to close.\n");
}