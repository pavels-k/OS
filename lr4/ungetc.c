#include <stdio.h>
#include <string.h>

int main(){
    char c;
    char command[10];
    while ((c = getchar()) != EOF){
        printf("c = %d\n", c);
        if (c == '\\'){
            printf("c = %d\n", c);
            if ((c = getchar()) != ' ' && c != '\n' && c != '\t'){
                printf("c = %d\n", c);
                ungetc(c, stdin); //
                printf("c = %d\n", c);
                scanf("%s", command);
                printf("c = %d\n", c);
                if (strcmp(command, "help") == 0){
                    printf("'%s' >>> \n", command);
                }
            }
        }
    }
    return 0;
}
