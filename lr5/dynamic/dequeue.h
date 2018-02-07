#include <stdio.h>
typedef struct elem elem;
struct elem
{
    elem* next;
    elem* prev;
    long int value;
};
typedef struct
{
    elem* begin;
    elem* end;
    size_t size;
}dequeue;
extern void push_back(dequeue*, long int);
extern void push_front(dequeue*, long int);
extern long int pop_back(dequeue*);
extern long int pop_front(dequeue*);
extern size_t size(dequeue*);