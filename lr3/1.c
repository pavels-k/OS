#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>



int main(int argc, char *argv[]) {
 srand(clock());

            int j = (int)rand() % (21);

    printf("%d\n", j);
    return 0;
}