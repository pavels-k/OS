#include "argument.h"
#include <stdlib.h>

void PrintResults(Result *tmp, int n);

int main(int argc, char **argv)
{
    srand((unsigned int) time(NULL));
    char answer[2048];
    Result table[10];
    int room_number;
    char address[25];
    void* context = zmq_ctx_new();
    
    if (!context)
    {
        perror("zmq_ctx_new");
        exit(1);
    }
    
    void* requester = zmq_socket(context, ZMQ_REQ);

    Args my_game;
    my_game.players = 0;
    my_game.status = -1;

    if (argc == 3 && !strcmp(argv[1], "-l"))
    {
        strcpy(my_game.log, argv[2]);
    }
    else
    {
        printf("usage: ./client -l <login>\n");
        return 0;
    }

    printf("Please enter the room number: ");
    scanf("%d", &room_number);
    getchar();
    
    sprintf(address, "%s%d", "tcp://localhost:", room_number);
    
    int cn = zmq_connect(requester, address);
    printf("Connect to tcp://localhost:%d\n", room_number);
    
    if (cn)
    {
        perror("connect");
        exit(1);
    }

    zmq_send(requester, &my_game, sizeof(Args), 0);
    zmq_recv(requester, &my_game, sizeof(Args), 0);

    if (my_game.players == 0)
    {
        printf("No free places\n");
        return 0;
    }

    while (1)
    {
        zmq_send(requester, &my_game, sizeof(Args), 0);
        zmq_recv(requester, &my_game, sizeof(Args), 0);
        
        if (my_game.status == 1)
        {
            break;
        }
    }
    
    printf(" =================================================\n");
    printf("                     RULES:                       \n\n");
    printf("     THERE ARE %d DICES OF SIZE %d TO BE ROLLED\n\n", my_game.dice_number, my_game.dice_size);
    printf("   THE ONE TO SCORE THE MOST POINTS IS THE WINNER\n\n");
    printf("                   GOOD LUCK!!!\n");
    printf("==================================================\n\n");
    
    my_game.result = 0;
    int cur_roll = 0;
    
    for (int i = 0; i < my_game.dice_number; ++i)
    {
        printf("Press ENTER to roll the dice ");
        getchar();
        
        cur_roll = rand() % my_game.dice_size;
        printf("You rolled %d\n", cur_roll);
        
        my_game.result += cur_roll;
    }
    
    printf("\nTOTAL SCORE: %d\n", my_game.result);

    my_game.status = 0;
    zmq_send(requester, &my_game, sizeof(Args), 0);
    zmq_recv(requester, &answer, sizeof(answer), 0);
    
    // printf("%s\n", answer);
    
    while (1)
    {
        int cmd;
        printf("1) Show results\n");
        printf("2) My stats\n");
        printf("3) TOP-3 \n");
        printf("4) Exit\n");
        scanf("%d", &cmd);
        
        switch (cmd)
        {
            case 1:
            {
                my_game.status = cmd;
                zmq_send(requester, &my_game, sizeof(Args), 0);
                zmq_recv(requester, &table, sizeof(Result) * 10, 0);
                
                if (table[0].threw)
                {
                    PrintResults(table, my_game.players);
                }
                
                break;
            }

            case 2:
            {
                my_game.status = cmd;
                zmq_send(requester, &my_game, sizeof(Args), 0);
                zmq_recv(requester, &answer, sizeof(answer), 0);
                printf("%s\n", answer); 
                
                break;
            }

            case 3:
            {
                my_game.status = cmd;
                zmq_send(requester, &my_game, sizeof(Args), 0);
                zmq_recv(requester, &answer, sizeof(answer), 0);
                printf("%s\n", answer);
                
                break;
            }

            case 4:
            {
                my_game.status = cmd;
                zmq_send(requester, &my_game, sizeof(Args), 0);
                zmq_recv(requester, &answer, sizeof(answer), 0);
                printf("%s\n", answer);
                
                zmq_close(requester);
                zmq_ctx_term(context);
                zmq_ctx_destroy(context);
                
                return 0;
                
                break;
            }

            default:
            {
                printf("ERROR: Invalid command\n");
                
                break;
            }
        }
    }

    return 0;
}

void PrintResults(Result *tmp, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d) %s: %d score\n", i + 1, tmp[i].name, tmp[i].score);
    }
    
    printf("\n");
}