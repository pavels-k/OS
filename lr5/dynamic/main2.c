#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "dequeue.h"
int main()
{
    void* library = NULL;
    library = dlopen("/home/libDYN_DQ.so",RTLD_LAZY); // Открыть библиотеку dl
    if(!library)
    {
        printf("%s\n",dlerror());
        exit(-1);
    }
    size_t(*size)(dequeue*) = dlsym(library,"size");
    void(*push_front)(dequeue*,long int ) = dlsym(library, "push_front");
    void(*push_back)(dequeue*, long int) = dlsym(library, "push_back");
    long int(*pop_back)(dequeue*) = dlsym(library, "pop_back");
    long int(*pop_front)(dequeue*) = dlsym(library, "pop_front");

    dequeue* dq = (dequeue*) malloc(sizeof(dequeue));
    dq->size = 0;
    dq->begin = NULL;
    dq->end = NULL;
    printf("%lu\n", (*size)(dq));
    (*push_front)(dq, 500);
    (*push_front)(dq, 600);
    (*push_front)(dq, 700);
    (*push_back)(dq, 500);
    (*push_back)(dq, 500);
    (*push_back)(dq, 500);
    printf("%lu\n", (*pop_front)(dq));
    printf("%lu\n", (*pop_front)(dq));
    printf("%lu\n", (*pop_back)(dq));
    printf("%lu\n", (*pop_back)(dq)); 
    free(dq);
    dlclose(library);
    
    return 0;
}