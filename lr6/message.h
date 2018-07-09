#ifndef message_h
#define message_h 

typedef struct _Msg{
    char action;
    double customer;
    int sum;
    double receiver;
    void* requester;
} MsgData;

#endif
