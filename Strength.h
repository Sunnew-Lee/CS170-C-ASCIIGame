/*
 * hyun.kang / sunwoo.lee
 * ASCII Game in C Assignment
 * CS170
 * Spring 2021
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
    int      Player_X, Player_Y;
    int      MovesCount;
    int      Width, Height;
    int      CurrentLevel;
    int*     Score;
    Tiles**  Map;
    time_t   Start;
    char     Myflag;
    char*    File_Name;
} Strength;

typedef void (*VoidFuncPtr)(char*);
typedef bool (*BoolFuncPtr)(char*);

void Set_Flag(char* myflag, VoidFuncPtr func);

bool Get_Flag(char* myflag, BoolFuncPtr func);

void Set_Isdone(char* myflag);

void Set_Iscleared(char* myflag);

bool Get_Isdone(char* myflag);
 
bool Get_Iscleared(char* myflag);

void Read_results();

void Game_setup(Strength* game);

void Map_setup(Strength* game);

void Create_map(Strength* game);

void Map_reset(Strength* game);

void Go_to_next_level(Strength* game);

void Write_results(Strength* game);

bool Game_update(Strength* game, int key);

void Draw_timer(Strength* game);

void Draw_map(Strength* game);