#include "argument.h"
#include "stats.h"


void Sorting(Result* sb, int n);
void PrintResults(Result* tmp, int n);
void GetTop(Scoreboard* db, char *ans);

int main(int argc, char **argv)
{
    int players_cnt;
    int id[10] = {0};
    int room_number;
    char address[25];
    char answer[2048];
    Result table[10];
    Args tmp;
    Args data[10];
    
    if (argc == 3 && !strcmp(argv[1],"-p"))
    {
        players_cnt = atoi(argv[2]);
    }
    else
    {
        printf("usage: ./server -p <number of players [2-10]>\n");
        return 0;
    }
    
    if (players_cnt < 2 || players_cnt > 10)
    {
        printf("usage: ./server -p <number of players [2-10]>\n");
        return 0;
    }

    void* context = zmq_ctx_new();
    
    if (!context)
    {
        perror("zmq_ctx_new");
        exit(1);
    }
    
    void* responder = zmq_socket(context, ZMQ_REP);
    
    printf("Enter the room number: ");
    scanf("%d", &room_number);
    
    sprintf(address, "%s%d", "tcp://*:", room_number);
    int bind = zmq_bind(responder, address);
    
    if (bind)
    {
        perror("bind");
        exit(1);
    }

    Scoreboard* db = Create();
    FILE *fp = fopen("SB", "r+");
    
    if (Load(&db, fp))
    {
        perror("db load");
        exit(1);
    }
    
    printf("Scoreboard:\n");
    Print(db);
    
    fseek(fp, 0, SEEK_SET);
    
    int dice_number = 0;
    int dice_size = 0;
    printf("Enter the size of dice: ");
    scanf("%d", &dice_size);
    if (dice_size < 1)
    {
        printf("use  the size of dice > 1\n");
        return 0;
    }
    
    printf("Enter the number of dices: ");
    scanf("%d", &dice_number);

    if (dice_number < 2)
    {
        printf("use  the number of dices > 2\n");
        return 0;
    }
    
    printf("Waiting for players to connect...\n");

    for (int i = 0; i < players_cnt; ++i)
    {
        zmq_recv(responder, &tmp, sizeof(Args), 0);
        
        if (!tmp.players)
        {
            printf("Player %s connected\n", tmp.log);
            tmp.checked = i;
            table[i].threw = 0;
            tmp.players = players_cnt;
            printf("\t%d players are ready\n", i + 1);
        }
        else
        {
            --i;
        }
        
        tmp.dice_size = dice_size;
        tmp.dice_number = dice_number;
        zmq_send(responder, &tmp, sizeof(Args), 0);
    }

    int num = players_cnt;
    
    printf("Start...\n");

    while (num)
    {
        zmq_recv(responder, &tmp, sizeof(Args), 0);
        
        if (!id[tmp.checked])
        {
            --num;
            tmp.status = 1;
            id[tmp.checked] = 1;
        }
        
        zmq_send(responder, &tmp, sizeof(Args), 0);
    }

    for (int i = 0; i < players_cnt; ++i)
    {
        id[i] = 0;
    }

    num = players_cnt;

    int data_cnt = 0;

    while (1)
    {   
        if (!players_cnt)
        {
            for (int i = 1; i < num; ++i)
            {
                for (int j = i; j > 0; --j)
                {
                    if (data[j - 1].result > data[j].result)
                    {
                        Args tmp = data[j - 1];
                        data[j - 1] = data[j];
                        data[j] = tmp;
                    }
                }
            }
            
            for (int i = 0; i < num - 1; ++i) // проигравшие
            {
                Update(db, data[i].log, 0);
            }
            
            Update(db, data[num - 1].log, 1); // выигравший
            
            Sorting(table, num);
            PrintResults(table, num);
            
            table[0].threw = 1;
            players_cnt = 1;
            
            Print(db);
            Save(db, fp);
            
            fclose(fp);
        }
        
        zmq_recv(responder, &tmp, sizeof(Args), 0);
        
        switch (tmp.status)
        {
            case 0:
            {
                if (!id[tmp.checked])
                {
                    strcpy(data[data_cnt].log, tmp.log);
                    data[data_cnt].result = tmp.result;
                    ++data_cnt;
                    
                    strcpy(table[tmp.checked].name, tmp.log);
                    table[tmp.checked].score = tmp.result;
                    id[tmp.checked] = 1;
                    
                    printf("User %s finished\n", tmp.log);
                    printf("His score: %d\n", tmp.result);
                    zmq_send(responder, &answer, sizeof(answer), 0);
                    
                    --players_cnt;
                }
                
                break;
            }

            case 1:
            {
                if (players_cnt)
                {
                    zmq_send(responder, &table, sizeof(table), 0);
                }
                else
                {
                    table[0].threw = 1;
                    zmq_send(responder, &table, sizeof(table), 0);
                }
                
                break;
            }

            case 2:
            {
                Player* print;
                print = Find(db, tmp.log);
                sprintf(answer, "Username: %s\nWin: %d\nLose: %d\n", print->log, print->wins, print->losses);
                zmq_send(responder, &answer, sizeof(answer), 0);
                
                break;
            }

            case 3:
            {
                GetTop(db, answer);
                zmq_send(responder, &answer, 2 * sizeof(answer), 0);
                
                break;
            }

            case 4:
            {
                printf("Player %s disconnected\n", tmp.log);
                strcpy(answer, "Goodbye!");
                zmq_send(responder, &answer, sizeof(answer), 0);
                
                break;
            }
                
            default:
            {
                tmp.players = 0;
                zmq_send(responder, &tmp, sizeof(Args), 0);
                
                break;
            }
        }

    }

    return 0;
}

void Sorting(Result *sb, int n)
{
    Result tmp;
    
    for (int i = 1; i < n; ++i)
    {
        for (int j = i; j > 0; --j)
        {
            if (sb[j - 1].score > sb[j].score)
            {
                tmp = sb[j - 1];
                sb[j - 1] = sb[j];
                sb[j] = tmp;
            }
        }
    }
}

void PrintResults(Result* tmp, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d) %s: %d score\n", i + 1, tmp[i].name, tmp[i].score);
    }
}

void GetTop(Scoreboard* db, char *ans)
{
    Sort(db);
    strcpy(ans, " ______________________________________\n");
    sprintf(ans, "%s|                                      |\n", ans);
    sprintf(ans, "%s| PLAYER              WINS      LOSSES |\n", ans);
    int size = (db->size > 3) ? 3 : db->size;

    for (int i = 0; i < size; ++i)
    {
        if (i == 0)
        {
            sprintf(ans, "%s|--------------------------------------|\n", ans);
        }
        else
        {
            sprintf(ans, "%s|                                      |\n", ans);
        }
        
        sprintf(ans, "%s| %-20s %-2d         %-2d   |\n", ans, db->players[i].log, db->players[i].wins, db->players[i].losses);
    }
    
    sprintf(ans, "%s|______________________________________|\n", ans);
}