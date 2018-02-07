#include<stdio.h>
#include <stdlib.h>
#include "dequeue.h"
int main()
{
    dequeue* dq = (dequeue*) malloc(sizeof(dequeue));
    dq->size = 0;
    dq->begin = NULL;
    dq->end = NULL;
    printf("%lu\n", size(dq));
    push_front(dq, 500);
    push_front(dq, 600);
    push_front(dq, 700);
    push_back(dq, 500);
    push_back(dq, 500);
    push_back(dq, 500);
    printf("%lu\n", pop_front(dq));
    printf("%lu\n", pop_front(dq));
    printf("%lu\n", pop_back(dq));
    printf("%lu\n", pop_back(dq)); 
    free(dq);
    return 0;
}