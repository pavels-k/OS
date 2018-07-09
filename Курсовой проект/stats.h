#ifndef _STATS_H_
#define _STATS_H_

#include <stdio.h>
#include <stdlib.h>
// #include <stdint.h>
// #include <inttypes.h>
#include <string.h>
#include "argument.h"

typedef struct _Player
{
    char log[256];
    int wins;
    int losses;
} Player;

typedef struct _Scoreboard
{
    Player* players;
    int size;
    int freespace;
} Scoreboard;

Scoreboard* Create(void);
void Add(Scoreboard* pl, char* log);
void Print(Scoreboard* pl);
Player* Find(Scoreboard* pl, char* log);
void Destroy(Scoreboard** pl);
void Sort(Scoreboard* pl);

int Save(Scoreboard* pl, FILE* file);
int Load(Scoreboard** pl, FILE* file);

void Fill(Scoreboard* pl, char* log, int wins, int loses);
void Update(Scoreboard* pl, char* log, int res);

#endif