#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "texteditor.h"
#include "parser.h"


void Usage(){
    if (fork() == 0){
        execl("/bin/cat", "cat", "usage", NULL);
    }
    wait(NULL);
    return;
}

void OC_InvCom(){
    printf("Invalid Command\n");
    if (fork() == 0){
        execl("/bin/cat", "cat", "help", NULL);
    }
    wait(NULL);
}

void Command(int argc, char** args){
    char c;
    char command[10];
    char emptystr[1];
    emptystr[0] = '\0';
    char word[10];
    char path[20];
    char* arg1;
    char* arg2;
    int min;
    int max;
    FILE *file = NULL, *newfile = NULL;
    
 // название файла
    printf("%s\n", args[1]);
    if (args[1][0] != '/'){
        Usage();
        return;
    }
    size_t i = 0;
    size_t shift = 1;
    while((c = args[1][i+shift]) != ':' && c != ' ' && c != '\t' && c != '\n' && c != EOF){
        word[i] = c;
        ++i;
    }
    word[i] = '\0';
    if (c != ':' || strcmp(word, "file") != 0){
        Usage();
        return;
    }
    
    shift += i+1;
    i = 0;
    while((c = args[1][i+shift]) != '\0'){
        path[i] = c;
        ++i;
    }
    path[i] = '\0';
    printf("File: %s\n", path); 
    if((file = fopen(path, "r")) == NULL){
        printf("File %s doesn't exist\n", path);
        return;
    }
    
 // название команды    
    for(size_t j = 2; j < argc; ++j){
        printf("%s\n", args[j]);
        if (args[j][0] != '/'){
            Usage();
            return;
        }
        size_t i = 0;
        size_t shift = 1;
        while((c = args[j][i+shift]) != ':' && c != ' ' && c != '\t' && c != '\n' && c != EOF){
            word[i] = c;
            ++i;
        }
        word[i] = '\0';
        if (c != ':' || strcmp(word, "command")){
            Usage();
            return;
        }
        
        shift += i+1;
        i = 0;
        while((c = args[j][i+shift]) != '\0' && c != ':'){
            command[i] = c;
            ++i;
        }
        command[i] = '\0';
        printf("~%s~\n", command); 

     // выполнение
        if (strcmp(command, "range") == 0){
            Range(file);
        }
        else if (strcmp(command, "help") == 0){
                    Help();
                }
        else if (strcmp(command, "exit") == 0){
            break;
        }
        else if (strcmp(command, "search") == 0){
            if(argc - j < 1){
                printf("ERROR: command 'open' has to have 1 argument!\n");
                return;
            }
            arg1 = args[++j];
            Search(file, arg1);
        }
        else if (strcmp(command, "replace") == 0){
            
            if (argc-j < 1){
                printf("ERROR: command 'replace' has to have at least 1 argument!\n");
                return;
            }
            arg1 = args[++j];
            if (argc-j == 1){
                arg2 = emptystr; 
            }
            else
                arg2 = args[++j];
            Replace(file, arg1, arg2);
            if (fork() == 0){ 
                execl("/bin/mv", "mv", "tmp", path, NULL);
            }
            wait(NULL);
            fopen(path, "r");
        }
        else if (strcmp(command, "limit") == 0){
            if(argc - j < 2){
                printf("ERROR: command 'open' has to have 2 argument!\n");
                return;
            }
            min = atoi(args[++j]);
            max = atoi(args[++j]);
            if (Limit(file, min, max)){
                printf("File %s exceeds the limit\n", path);
                break;
            }
            else printf("oK\n");
        }
        else if (strcmp(command, "open") == 0){
            if(argc - j < 1){
                printf("ERROR: command 'open' has to have 1 argument!\n");
                return;
            }
            arg1 = args[++j];				
            if((newfile = fopen(arg1, "r")) == NULL){
                printf("File %s doesn't exist\n", arg1);
                fclose(newfile);
            }
            else{
                file = newfile;
                strcpy(path, arg1);
                printf("File %s has been opened successfully\n", path);
            }
        }
        else{
            OC_InvCom();
        }
    }
    fclose(file);
}
