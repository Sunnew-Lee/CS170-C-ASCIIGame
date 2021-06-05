/*
    sunwoo.lee
    ASCII Game
    CS170
    Spring 2021
*/
#pragma once

#include <stdbool.h>
#include <time.h>

extern char IsDone; 
extern char IsCleared; 

typedef enum
{
    ROAD,         
    WALL,    
    ROCK, 
    PIT, 
    NEXT_LEVEL 
} Tiles;

typedef struct
{
    int      PlayerX, PlayerY;
    int      MovesCount;
    int      Width, Height;
    int      CurrentLevel;
    unsigned int*     Score;
    Tiles**  Map;
    time_t   Start;
    char     Myflag;
    char*    File_Name;
} Strength;

typedef void (*FuncPtr)(Strength*);

void Set_Flag(Strength* game, FuncPtr func);

void Set_Isdone(Strength* game);

void Set_Iscleared(Strength* game);
 
void Game_intro();

void Game_setup(Strength* game);

void Map_setup(Strength* game);

void Create_map(Strength* game);

void Map_reset(Strength* game);

void Go_to_next_level(Strength* game);

void Write_results(Strength* game);

bool Game_update(Strength* game, int key_code);

void Draw_timer(Strength* game);

void Draw_map(Strength* game);