#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <stdio.h>
#include <stdbool.h>

void Range(FILE*);
void Search(FILE*,char*);
void Print(char*);
void Replace(FILE*,char*,char*);
bool Limit(FILE* file, int min, int max);

#endif