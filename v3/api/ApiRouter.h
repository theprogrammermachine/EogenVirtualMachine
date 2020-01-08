
#include <openssl/sha.h>
#include "IO/ConsolePrinter.h"
#include "../Models.h"

void* routeAndResolve(char* funcRef, Dictionary* entries) {
    if (strcmp(funcRef, "print") == 0) {
        ConsolePrinter printer = createConsolePrinter();
        void* v = dict_get(entries, "text");
        if (strcmp((*(Value*)v).valueType, "string") == 0) {
            printer.print((*(StringValue*)v).value);
        } else if (strcmp((*(Value*)v).valueType, "short") == 0) {
            ShortValue val1 = *(ShortValue*)v;
            char str[(int)(ceilf(log10f(val1.value)+1)*sizeof(char))];
            sprintf(str, "%hd", val1.value);
            char* str2 = &str[0];
            printer.print(str2);
        }
        else if (strcmp((*(Value*)v).valueType, "int") == 0) {
            IntValue val1 = *(IntValue*)v;
            char str[(int)(ceil(log10(val1.value)+1)*sizeof(char))];
            sprintf(str, "%d", val1.value);
            char* str2 = &str[0];
            printer.print(str2);
        }
        else if (strcmp((*(Value*)v).valueType, "long") == 0) {
            LongValue val1 = *(LongValue*)v;
            char str[(int)(ceill(log10l(val1.value)+1)*sizeof(char))];
            sprintf(str, "%ld", val1.value);
            char* str2 = &str[0];
            printer.print(str2);
        }
        else if (strcmp((*(Value*)v).valueType, "float") == 0) {
            FloatValue val1 = *(FloatValue*)v;
            char str[(int)(ceilf(log10f(val1.value)+1)*sizeof(char))];
            sprintf(str, "%f", val1.value);
            char* str2 = &str[0];
            printer.print(str2);
        }
        else if (strcmp((*(Value*)v).valueType, "double") == 0) {
            DoubleValue val1 = *(DoubleValue*)v;
            char str[(int)(ceil(log10(val1.value)+1)*sizeof(char))];
            sprintf(str, "%f", val1.value);
            char* str2 = &str[0];
            printer.print(str2);
        }
        static StringValue value;
        value.base.valueType = "string";
        value.value = "done.";
        return &value;
    }
    else if (strcmp(funcRef, "sha256") == 0) {

    }
}