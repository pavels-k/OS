#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
void Skip(){
    char c;
    while ((c = getchar()) != '\n'){}
    return;
}

int main(int argc, char *argv[])
{
    char path[10];
    FILE *file = NULL;
    FILE* newfile = fopen("tmp", "w");
    while(true){
        printf("Enter the path of the file: ");
        scanf("%s", path);
        Skip();
        if((file = fopen(path, "r")) == NULL){
            printf("File %s doesn't exist\n", path);
        }
        else{
                printf("File %s has been opened successfully\n", path);
                break;
            }
    }
    // if (fork() == 0){
    //     execl("/bin/mv", "mv", "tmp", path, NULL);
    // }
     printf("kek\n");
     getchar();
    // wait(NULL);
    fopen(path, "r");
}
