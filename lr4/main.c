// вывести диапазон символов
// поиск по подстроке
// замена фрагмента строки на другую часть строки
// возможность задания лимита размера файла

#include "parser.h"
#include "command.h"

int main(int argc, char* argv[]){
    if (argc == 1)
        Parser();
    else
        Command(argc,argv); // non-interactive
    return 0;
}