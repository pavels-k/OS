#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define STRING_SIZE 1000
#define BUFFER_SIZE 1000

//this function converts a string from a file to an array of integer numbers and returns its size
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

//this function converts an array of integer numbers to a string
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

//just a simple bubble sort
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

#endif /* FUNCTIONS_H */
