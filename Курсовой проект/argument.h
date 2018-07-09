#ifndef ARG_H
#define ARG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <zmq.h>

typedef struct _Result
{
    char name[256];
    int score;
    int threw;
} Result;

typedef struct _Args
{
    char log[256];
    int dice_size;
    int dice_number;
    int checked;
    int players;
    void* requester;
    int status;
    int result;
} Args;

#endif