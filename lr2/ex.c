#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
// #include <time.h>
// #include <sys/types.h>
// #include <sys/stat.h>

#define STRING_SIZE 1000
#define BUFFER_SIZE 1000

// эта функция преобразует строку из файла в массив целых чисел и возвращает его размер
int StringToInt(int* buffer, char* string, int string_size)
{
    int i = 0, buffer_size = 0;
    for (i = 0; i < string_size; i++) {
        if (string[i] == ' ' || string[i] == '\n') {
            buffer_size++;
        } else if (string[i] == '-') {
            buffer[buffer_size] = -1 * (string[i+1] - '0');
            i++;
        } else {
            if (buffer[buffer_size] >= 0) {
                buffer[buffer_size] = buffer[buffer_size] * 10 + (string[i] - '0');
            } else {
                buffer[buffer_size] = buffer[buffer_size] * 10 - (string[i] - '0');
            }
        }
    }
    return buffer_size;
}
// эта функция преобразует массив целых чисел в строку
void IntToString(int* buffer, int buffer_size, char* string)
{
    int i = 0, string_size = 0;
    for (i = 0; i < STRING_SIZE; i++) {
        string[i] = '\0';
    }
    for (i = 0; i < buffer_size; i++) {
        string_size += sprintf(string + string_size, "%d", buffer[i]);
        string[string_size] = ' ';
        string_size++;
    }
}
// простая сортировка пузырьком
void BubbleSort(int* buffer, int buffer_size)
{
    int i = 0, j = 0, tmp = 0;
    for (i = 0; i < buffer_size - 1; i++) {
        for (j = 0; j < buffer_size - 1; j++) {
            if (buffer[j] > buffer[j+1]) {
                tmp = buffer[j];
                buffer[j] = buffer[j+1];
                buffer[j+1] = tmp;
            }
        }
    }
}

int main(int argc, char* argv[]) {

    //строка
    char string[STRING_SIZE] = "H A ello wo rld";
    int string_size = 12;
     printf("%s\n", string);
    char c = '\0';

    //buffer
    int buffer[BUFFER_SIZE] = {0};
    int buffer_size = 0;

    // создание массива целых чисел из файла
    buffer_size = StringToInt(buffer, string, string_size);

    //сортировка
    BubbleSort(buffer, buffer_size);

    // обратное преобразование
    IntToString(buffer, buffer_size, string);
         printf("%s\n", string);

    return 0;
}
