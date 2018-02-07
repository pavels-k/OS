#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// вывести дипапозон
void Range(FILE* file){ 
    char c[1], min = CHAR_MAX, max = CHAR_MIN;
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    for(size_t i = 0; i < file_size; ++i){
        fread(c, 1, 1, file);
        if (c[0] < min && c[0] > ' ') min = c[0];
        if (c[0] > max) max = c[0];
    }
    printf("Range: min='%c', max='%c'\n", min, max);
    return;
}

// поиск
void Search(FILE* file, char* pattern){
    size_t patt_size = strlen(pattern);
    char compare[patt_size+1];
    size_t count = 0;
    
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size < patt_size) return;
    for(size_t i = 0; i <= file_size-patt_size; ++i){
        fread(compare, patt_size, 1, file);
        if (strcmp(compare, pattern) == 0)
            ++count;
        fseek(file, i+1, SEEK_SET);
    }
    printf("Number of coincidences: %lu\n", count);
}

// замена
void Replace(FILE* file, char* pattern, char* replace){
    FILE* newfile = fopen("tmp", "w");
    size_t patt_size = strlen(pattern);
    char compare[patt_size+1];
    bool coincidence = false;
    size_t count = 0;
    
    size_t repl_size = strlen(replace);
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < patt_size) return;
    while(file_size-ftell(file) >= patt_size){
        fread(compare, patt_size, 1, file);
        compare[patt_size] = '\0';
        if (strcmp(compare, pattern) == 0){
            fwrite(replace, repl_size, 1, newfile);
            coincidence = true;
            count += patt_size;
        }
        else{
            fwrite(compare, 1, 1, newfile);        
            fseek(file, ++count, SEEK_SET);
            coincidence = false;
        }
    }
    size_t position = ftell(file);
    if (!coincidence || position < file_size){
        fread(compare, file_size-position, 1, file);
        fwrite(compare, file_size-position, 1, newfile);
    }
    fclose(newfile);
    fclose(file);
}

// лимит
bool Limit(FILE* file, int min, int max) {
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    if(file_size >= min && file_size <= max) return false;
    else return true;
}