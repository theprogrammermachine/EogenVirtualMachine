
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void* print(char* text) {
    printf("%s", text);
}

typedef struct {
    void* (*print)(char* text);
} ConsolePrinter;

ConsolePrinter createConsolePrinter() {
    ConsolePrinter printer;
    printer.print = print;
    return printer;
}