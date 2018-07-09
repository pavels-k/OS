#include "stats.h"

Scoreboard* Create(void)
{
    Scoreboard* pl = (Scoreboard*) malloc(sizeof(Scoreboard));
    
    if (!pl)
    {
        fprintf(stderr, "ERROR: no memory\n");
        exit(1);
    }
    
    pl->players = (Player*) malloc(sizeof(Player) * 10);
    pl->size = 0;
    pl->freespace = 10;
    
    return pl;
}

int Load(Scoreboard** pl, FILE* file)
{
    Player tmp;
    
    if (file < 0)
    {
        printf("Cannot open file\n");
        return 1;
    }
    
    while (fscanf(file, "%s %d %d", tmp.log, &tmp.wins, &tmp.losses) == 3) {
        Add((*pl), tmp.log);
        Fill((*pl), tmp.log, tmp.wins, tmp.losses);
    }
    return 0;
}

void Add(Scoreboard* pl, char *log)
{
    strcpy(pl->players[pl->size].log, log);
    pl->players[pl->size].wins = 0;
    pl->players[pl->size].losses = 0;
    pl->size++;
    pl->freespace--;
}

void Fill(Scoreboard* pl, char *log, int win, int lose)
{
    Player* toFill = Find(pl, log);
    
    if (toFill)
    {
        toFill->wins = win;
        toFill->losses = lose;
    }
}

Player* Find(Scoreboard* pl, char *log)
{
    if (pl)
    {
        for (int i = 0; i < pl->size; ++i)
        {
            if (!strcmp(pl->players[i].log, log))
            {
                return &(pl->players[i]);
            }
        }
    }
    
    return NULL;
}

void Print(Scoreboard* pl)
{
    if (pl)
    {
        printf(" ______________________________________\n");
        printf("|                                      |\n");
        printf("| PLAYER              WINS      LOSSES |\n");
        
        for (int i = 0; i < pl->size; ++i)
        {
            if (i == 0)
            {
                printf("|--------------------------------------|\n");
            }
            else
            {
                printf("|                                      |\n");
            }
            
            printf("| %-20s %-2d         %-2d   |\n", pl->players[i].log, pl->players[i].wins, pl->players[i].losses);
        }
        
        printf("|______________________________________|\n");
    }
}

void Update(Scoreboard* pl, char* log, int res)
{
    Player* player = Find(pl, log);
    
    if (player == NULL)
    {
        Add(pl, log);
        player = Find(pl, log);
    }
    
    res == 1 ? player->wins++ : player->losses++;
}



int Save(Scoreboard* pl, FILE* file)
{
    if (file < 0)
    {
        printf("Cannot open file\n");
        return 1;
    }
    
    for (int i = 0; i < pl->size; ++i)
    {
        fprintf(file, "%s %d %d ", pl->players[i].log, pl->players[i].wins, pl->players[i].losses);
    }
    
    return 0;
}
void Destroy(Scoreboard** pl)
{
    free((*pl)->players);
    (*pl)->players = NULL;
    
    free(*pl);
    *pl = NULL;
}

void Sort(Scoreboard* pl)
{
    struct _Player tmp;
    
    if (pl)
    {
        for (int i = 1; i < pl->size; ++i)
        {
            for (int j = i; j > 0; --j)
            {
                if (pl->players[j - 1].wins < pl->players[j].wins)
                {
                    tmp = pl->players[j - 1];
                    pl->players[j - 1] = pl->players[j];
                    pl->players[j] = tmp;
                }
            }
        }
    }
}