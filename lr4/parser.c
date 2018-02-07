#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "texteditor.h"

void Skip(){
    char c;
    while ((c = getchar()) != '\n'){}
    return;
}

void InvCom(){
    printf("ERROR: Invalid command. Enter \\help to see the list of commands\n");
    return;
}

void Help(){
    if (fork() == 0){
        execl("/bin/cat", "cat", "help", NULL);
    }
    wait(NULL);
    return;
}

void Parser(){
    char c;
    char pattern[20];
    char substit[20];
    char command[10];
    char path[10];
    char newpath[10];
    int min = 0;
    int max = 1000;
    FILE *file = NULL;
    FILE *newfile = NULL;
    
    while(true){
        printf("Enter the path of the file: ");
        scanf("%s", path);
        Skip();
        if((file = fopen(path, "r")) == NULL){
            printf("File %s doesn't exist\n", path);
        }
        else{
            if (Limit(file, min, max)){
                printf("File %s exceeds the limit\n", path);
                fclose(file);
            }
            else{
                printf("File %s has been opened successfully\n", path);
                break;
            }
        }
    }
    
    printf("'%s' >>> ", path);
    while ((c = getchar()) != EOF){ 
        if (c == '\\'){
            if ((c = getchar()) != ' ' && c != '\n' && c != '\t'){
                ungetc(c, stdin);
                scanf("%s", command);
                if (strcmp(command, "help") == 0){
                    Help();
                }
                else if (strcmp(command, "range") == 0){
                    Range(file);
                }
                else if (strcmp(command, "setlimit") == 0){
                    scanf("%d%d", &min, &max);
                    if (Limit(file, min, max)){
                        printf("File %s exceeds the limit\n", path);
                        break; 
                    }
                }
                else if (strcmp(command, "exit") == 0){
                    break;
                }
                else if (strcmp(command, "search") == 0){
                    scanf("%s", pattern);
                    Search(file, pattern);
                }
                else if (strcmp(command, "replace") == 0){
                    scanf("%s", pattern);
                    while ((c = getchar()) == ' '){}
                    if (c == '\n')
                        substit[0] = '\0';
                    else{
                        ungetc(c, stdin);
                        scanf("%s", substit);
                    }
                    Replace(file, pattern, substit);
                    if (fork() == 0){
                        execl("/bin/mv", "mv", "tmp", path, NULL);
                    }
                    wait(NULL);
                    fopen(path, "r");
                }
                else if (strcmp(command, "open") == 0){
                    scanf("%s", newpath);				
                    if((newfile = fopen(newpath, "r")) == NULL){
                        printf("File %s doesn't exist\n", newpath);
                        fclose(newfile);
                    }
                    else{
                        if(Limit(file, min, max))
                            printf("File %s exceeds the limit\n", path);
                        else{
                            file = newfile;
                            strcpy(path, newpath);
                            printf("File %s has been opened successfully\n", path);
                        }
                    }
                }
                
                else{
                    InvCom();
                }
                if (c != '\n')  Skip();
            }
            else{
                InvCom();
                if (c != '\n') Skip();
            }
        }
        else{
            InvCom();
            if (c != '\n') Skip();
        }
        printf("'%s' >>> ", path);
    }
    fclose(file);
}