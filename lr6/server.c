#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <zmq.h>
#include "bank.h"
#include "message.h"


volatile sig_atomic_t wait = 1;

void Stop(int sig){
    if (wait == 1){
        printf("\nОстанавливаемся\n");
        wait = 0;
    }
    else{
        printf("\nВыходим\n");
        wait = -1;
    }
}

void Resumption(int sig){
    if (wait != 1){
        printf("\nВозобновляем\n");
        wait = 1;
    }
}


int main(){
    double bank;
    char address[25];
    Vault* customer_base = NULL;
    int exit_status;
    void* context = zmq_ctx_new();
    void* response_socket = zmq_socket(context, ZMQ_REP);

    printf("Введите адрес банка: ");
    scanf("%lf", &bank);
    sprintf(address, "%s%lf", "tcp://*:", bank);

    zmq_bind(response_socket, address); //

    while (1){

        if (wait == -1){
            break;
        }

        // if (wait){
        zmq_msg_t message;

        signal(SIGINT, Stop);
        signal(SIGTSTP, Resumption);

        zmq_msg_init(&message);
        zmq_msg_recv(&message, response_socket, 0);

        if (wait){

            MsgData* msg_data = (MsgData*) zmq_msg_data(&message); // из message

            zmq_msg_close(&message);

            switch (msg_data->action){
            case '1':{
                printf("Положить деньги на аккаунт ID: %lf\n", msg_data->customer);

                MoneyPut(msg_data->customer, msg_data->sum, &customer_base);
                VaultCustomerPrint(&customer_base, msg_data->customer);

                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), msg_data, sizeof(MsgData));

                zmq_msg_send(&message, response_socket, 0);
                zmq_msg_close(&message);

                break;
            }

            case '2':{
                printf("Снять деньги с аккаунта ID: %lf\n", msg_data->customer);

                exit_status = MoneyWithdraw(msg_data->customer, msg_data->sum, &customer_base);

                if (exit_status == SUCCESS){
                    VaultCustomerPrint(&customer_base, msg_data->customer);
                }
                else if (exit_status == NOT_ENOUGH_MONEY){
                    printf("Недостаточно денег\n");
                }
                else{
                    printf("Это не клиент банка\n");
                }

                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), msg_data, sizeof(MsgData));

                zmq_msg_send(&message, response_socket, 0);
                zmq_msg_close(&message);

                break;
            }

            case '3':{
                printf("Перевести деньги с аккаунта ID: %lf на аккаунт ID: %lf\n", msg_data->customer, msg_data->receiver);

                exit_status = MoneyTransfer(msg_data->customer, msg_data->receiver, msg_data->sum, &customer_base);

                if (exit_status == SUCCESS){
                    VaultCustomerPrint(&customer_base, msg_data->customer);
                    VaultCustomerPrint(&customer_base, msg_data->receiver);

                }
                else if (exit_status == IS_NOT_CUSTOMER){
                    printf("Отправитель не клиент банка\n");
                }
                else if (exit_status == NOT_ENOUGH_MONEY){
                    printf("Недостачно денег\n");
                }
                else if (exit_status == RECEIVER_IS_NOT_CUSTOMER){
                    printf("Получатель не клиент банка\n");
                }

                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), msg_data, sizeof(MsgData));

                zmq_msg_send(&message, response_socket, 0);
                zmq_msg_close(&message);

                break;
            }

            case '4':{
                printf("Проверить баланс аккаунта ID: %lf\n", msg_data->customer);

                exit_status = CheckAccount(msg_data->customer, &customer_base);

                if (exit_status == IS_NOT_CUSTOMER){
                    printf("Не клиента банка\n");
                }
                else{
                    VaultCustomerPrint(&customer_base, msg_data->customer);
                }

                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), msg_data, sizeof(MsgData));

                zmq_msg_send(&message, response_socket, 0);
                zmq_msg_close(&message);

                break;
            }

                zmq_msg_close(&message);
            }
        }
    }

    zmq_close(response_socket);
    zmq_ctx_destroy(context);

    VaultDestroy(&customer_base);
}