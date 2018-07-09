#ifndef BANK_H
#define BANK_H 

#include <stdio.h>
#include <stdlib.h>

typedef struct _CustData{
    double id;
    int sum;
} CustData;

typedef struct _Vault{
    CustData customer;
    struct _Vault* left;
    struct _Vault* right;
} Vault;

enum STATE{
    SUCCESS = 1,
    IS_NOT_CUSTOMER = -1,
    NOT_ENOUGH_MONEY = -2,
    RECEIVER_IS_NOT_CUSTOMER = -3
};

void VaultInsert(Vault** v, double id);
Vault* VaultFind(Vault** v, double id);
void VaultCustomerPrint(Vault** v, double id);
void VaultDestroy(Vault** v);
int HasEnoughMoney(CustData* cust, int sum);
void MoneyPut(double id, int sum, Vault** v);
int MoneyWithdraw(double id, int sum, Vault** v);
int MoneyTransfer(double sender_id, double receiver_id, int sum, Vault** v);
int CheckAccount(double id, Vault** v);

#endif