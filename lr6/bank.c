#include "bank.h"

void VaultInsert(Vault** v, double id){
    
    if (*v == NULL){
        *v = (Vault*) malloc(sizeof(Vault));
        
        (*v)->customer.id = id;
        (*v)->left = NULL;
        (*v)->right = NULL;
        (*v)->customer.sum = 0; 
    }
    else if (id > (*v)->customer.id){
        return VaultInsert(&(*v)->right, id);
    }
    else if (id < (*v)->customer.id){
        return VaultInsert(&(*v)->left, id);
    }
}

void VaultCustomerPrint(Vault** v, double id){
    Vault* cust = VaultFind(v, id);
    
    if (cust == NULL){        
        return;
    }
    
    printf("ID: %lf\t", cust->customer.id);
    printf("Баланс аккаунта: %d\n", cust->customer.sum);
}

Vault* VaultFind(Vault** v, double id){
    if (*v == NULL){
        return NULL;
    }
    else if (id > (*v)->customer.id){
        return VaultFind(&(*v)->right, id);
    }
    else if (id < (*v)->customer.id){
        return VaultFind(&(*v)->left, id);
    }
    
    return *v;
}


int HasEnoughMoney(CustData* cust, int sum){
    return cust->sum >= sum;
}

void MoneyPut(double id, int sum, Vault** v){
    Vault* cust = VaultFind(v, id);
    
    if (cust == NULL){
        VaultInsert(v, id);
        cust = VaultFind(v, id);
    }
    
    cust->customer.sum += sum;
}

int MoneyWithdraw(double id, int sum, Vault** v){
    Vault* cust = VaultFind(v, id);
    
    if (cust == NULL){
        return IS_NOT_CUSTOMER;
    }
    
    if (HasEnoughMoney(&cust->customer, sum)){
        cust->customer.sum -= sum;
        
        return SUCCESS;
    }
    else{
        return NOT_ENOUGH_MONEY;
    }
}

int MoneyTransfer(double sender_id, double receiver_id, int sum, Vault** v){
    Vault* sender = VaultFind(v, sender_id);
    
    if (sender == NULL){
        return IS_NOT_CUSTOMER;
    }
    
    Vault* receiver = VaultFind(v, receiver_id);
    
    if (receiver == NULL){
        return RECEIVER_IS_NOT_CUSTOMER;
    }
    
    if (HasEnoughMoney(&sender->customer, sum)){
        sender->customer.sum -= sum;
        receiver->customer.sum += sum;
        
        return SUCCESS;
    }
    else{
        return NOT_ENOUGH_MONEY;
    }
}

int CheckAccount(double id, Vault** v){
    Vault* cust = VaultFind(v, id);
    
    if (cust == NULL){
        return IS_NOT_CUSTOMER;
    }
    
    return cust->customer.sum;
}

void VaultDestroy(Vault** v){
    if (*v == NULL){
        return;
    }
    
    VaultDestroy(&(*v)->left);
    VaultDestroy(&(*v)->right);
    
    free(*v);
    
    *v = NULL;
}