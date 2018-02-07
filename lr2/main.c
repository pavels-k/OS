// На вход программе подается название 2-ух файлов. 
// Необходимо отсортировать оба файла (каждый в отдельном процессе)
// произвольной сортировкой (на усмотрение студента). 
// Содержимое обоих файлов вывести в стандартный поток вывода родительским процессом.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "Functions.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong arguments!\n");
        return 1;
    }

    pid_t pid_1 = 0, pid_2 = 0;
    int exit_status = 0;

    int fd_1[2];
    pipe(fd_1);
    int fd_2[2];
    pipe(fd_2);

    char string[STRING_SIZE] = {'\0'};
    int string_size = 0;
    char c = '\0';
    int buffer[BUFFER_SIZE] = {0};
    int buffer_size = 0;

    if ((pid_1 = fork()) == 0) {

        close(fd_1[0]); // не понадобится выходной поток
        int file_1 = 0;
        if ((file_1 = open(argv[1], O_RDWR)) == -1) { // O_RDWR — для чтения и записи
            printf("%s: no such file!\n", argv[1]);
            exit(1); // выйдет из дочернего процесса
        }

        while (read(file_1, &c, 1)) {
            string[string_size] = c;
            string_size++;
        }

        buffer_size = StringToInt(buffer, string, string_size);
        BubbleSort(buffer, buffer_size);
        IntToString(buffer, buffer_size, string);

        write(fd_1[1], string, sizeof(string));
        close(fd_1[1]);

        exit(0);
    }

    //написать результат если exit_status == 0
    wait(&exit_status);
    if (WEXITSTATUS(exit_status) == 0) { // WEXITSTATUS возвращает код завершения потомка.
        close(fd_1[1]);
        read(fd_1[0], string, sizeof(string));
        printf("%s\n", string);
        close(fd_1[0]);
    }

    if ((pid_2 = fork()) == 0) {

        close(fd_2[0]); // не понадобится выходной поток
        int file_2 = 0;
        if ((file_2 = open(argv[2], O_RDWR)) == -1) {
            printf("%s: no such file!\n", argv[2]);
            exit(1);
        }

        while (read(file_2, &c, 1)) {
            string[string_size] = c;
            string_size++;
        }

        buffer_size = StringToInt(buffer, string, string_size);
        BubbleSort(buffer, buffer_size);
        IntToString(buffer, buffer_size, string);

        write(fd_2[1], string, sizeof(string));
        close(fd_2[1]);

        exit(0);
    }

    //написать результат если exit_status == 0
    wait(&exit_status);
    if (WEXITSTATUS(exit_status) == 0) { // WEXITSTATUS возвращает код завершения потомка.
        close(fd_2[1]);
        read(fd_2[0], string, sizeof(string));
        printf("%s\n", string);
        close(fd_2[0]);
    }

    return 0;
}