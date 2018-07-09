#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include "message.h"
 
void Send(MsgData* msg_data){
    
    zmq_msg_t message;
    zmq_msg_init_size(&message, sizeof(MsgData));
    memcpy(zmq_msg_data(&message), msg_data, sizeof(MsgData));
    zmq_msg_send(&message, msg_data->requester, 0);
    zmq_msg_close(&message);
    
    zmq_msg_init(&message);
    zmq_msg_recv(&message, msg_data->requester, 0);
    
    msg_data = (MsgData*) zmq_msg_data(&message);
    
    zmq_msg_close(&message);
    
    
    return;
}

int main(){
    void* context = zmq_ctx_new(); // Создаёт контекст
    void* send_socket = zmq_socket(context, ZMQ_REQ); // Создаёт новый сокет в context

    double id = 0.0;
    double bank = 0.0;

    printf("Введите свой ID: ");
    scanf("%lf", &id);

    char address[255];
    
    printf("Введите адрес банка: ");
    scanf("%lf", &bank);

    sprintf(address, "%s%lf", "tcp://localhost:", bank);
    printf("tcp://localhost:%lf \n", bank);
    zmq_connect(send_socket, address);
    char action = '\0';
    
    printf("Меню:\n");
    printf("1) Положить деньги в банк\n");
    printf("2) Взять деньги из банка\n");
    printf("3) Перевести деньги на другой аккаунт\n");
    printf("4) Проверить баланс аккаунта\n");
    printf("5) Выход\n");
    
    while (1){
        scanf(" %c", &action);
        
        if (action == '5'){
            break;
        }
        
        MsgData msg_data; // из message.h
        msg_data.action = action;
        msg_data.customer = id;
        msg_data.sum = 0;
        
        int amount = 0;

        switch (action){

            case '1':{
                printf("Введите сумму: ");
                scanf("%d", &amount);

                msg_data.sum = amount;
                msg_data.requester = send_socket; 
                Send(&msg_data); // функция
                
                break;
            }

            case '2':{
                printf("Введите сумму: ");
                scanf("%d", &amount);

                msg_data.sum = amount;
                msg_data.requester = send_socket;
                Send(&msg_data);

                break;
            }

            case '3':{
                double receiver_id;
                
                printf("Введите номер получателя: ");
                scanf("%lf", &receiver_id);

                printf("Введите сумму: ");
                scanf("%d", &amount);

                msg_data.sum = amount;
                msg_data.receiver = receiver_id;
                msg_data.requester = send_socket;
                Send(&msg_data);

                break;
            }

            case '4':{
                msg_data.requester = send_socket;
                Send(&msg_data);


                break;
            }
    
            default:{
                printf("ERROR: Invalid action\n");
                break;
            }
        }
    }

    zmq_close(send_socket);
    zmq_ctx_destroy(context);

    return 0;
}