
#include "Stack.h"
#include <math.h>
#include <float.h>
#include "api/ApiRouter.h"
#include <math.h>

unsigned long codeLength = 0;
char* code;
int machineState = 0x00;
unsigned long pointer = 0;

struct Stack codeLengthStack;
struct Stack bufferStack;
struct Stack expStack;
struct Stack dataStack;

void* executeIntern(char c[], unsigned long length, Dictionary* entriesDict);
void calculate(int investigateId);
void* ride();
void execute(char* c, long length);

void initStack(struct Stack* stack) {
    stack->item = NULL;
    stack->stackSize = 0;
    stack->push = push;
    stack->pop = pop;
    stack->top = top;
    stack->size = size;
    stack->isEmpty = isEmpty;
    stack->iterator = iterator;
}

void initList(struct List* list) {
    list->size = 0;
    list->listPointer = NULL;
    list->iteratorPointer = NULL;
    list->append = listAdd;
    list->iteratorForward = iteratorForward;
    list->iteratorHasNext = iteratorHasNext;
    list->iteratorBackward = iteratorBackward;
    list->iteratorHasBefore = iteratorHasBefore;
}

char* concat(const char *s1, char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void* sum(void* value1, void* value2) {
    Value val1Raw = *(Value*)value1;
    Value val2Raw = *(Value*)value2;
    if (strcmp(val1Raw.valueType, "int") == 0 ||
        strcmp(val1Raw.valueType, "short") == 0 ||
        strcmp(val1Raw.valueType, "long") == 0 ||
        strcmp(val1Raw.valueType, "float") == 0 ||
        strcmp(val1Raw.valueType, "double") == 0) {
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            long double result = 0;
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue*)value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue*)value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue*)&value2;
                    result = (long double)val1.value + (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue*)&value2;
                    result = (long double)val1.value + (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue*)&value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue*)&value2;
                    result = (long double)val1.value + (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue*)&value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue*)&value2;
                    result = (long double) val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue*)&value2;
                    result = (long double) val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue*)&value2;
                    result = (long double) val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue*)&value2;
                    result = (long double) val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue*)&value2;
                    result = (long double) val1.value + (long double)val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue*)&value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue*)&value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue*)&value2;
                    result = (long double)val1.value + (long double)val2.value;
                }
            }
            if (floorl(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue* resValue = malloc(sizeof(ShortValue));
                    short r = (short)result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    IntValue* resValue = malloc(sizeof(IntValue));
                    int r = (int)result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    LongValue* resValue = malloc(sizeof(LongValue));
                    long r = (long)result;
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    FloatValue* resValue = malloc(sizeof(FloatValue));
                    float r = (float)result;
                    resValue->value = r;
                    resValue->base.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    DoubleValue* resValue = malloc(sizeof(DoubleValue));
                    double r = (double)result;
                    resValue->value = r;
                    resValue->base.valueType = "double";
                    return resValue;
                }
            }
        }
        else if (strcmp(val2Raw.valueType, "string") == 0) {
            StringValue val2 = *(StringValue*)value2;
            char* str2 = "";
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue*)value1;
                char str[(int)(ceil(log10(val1.value)+1)*sizeof(char))];
                sprintf(str, "%hd", val1.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue*)value1;
                char str[(int)(ceil(log10(val1.value)+1)*sizeof(char))];
                sprintf(str, "%d", val1.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue*)value1;
                char str[(int)(ceill(log10l(val1.value)+1)*sizeof(char))];
                sprintf(str, "%ld", val1.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val1Raw.valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue*)value1;
                char str[(int)(ceilf(log10f(val1.value)+1)*sizeof(char))];
                sprintf(str, "%f", val1.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val1Raw.valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue*)value1;
                char str[(int)(ceil(log10(val1.value)+1)*sizeof(char))];
                sprintf(str, "%f", val1.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            }
            StringValue* resValue = malloc(sizeof(StringValue));
            char* result = concat(str2, val2.value);
            resValue->value = result;
            resValue->base.valueType = "string";
            return resValue;
        }
        else if (strcmp(val2Raw.valueType, "bool") == 0) {
            StringValue val2 = *(StringValue*)value2;
            double result = 0;
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue*)value1;
                result = (double)val1.value + (val2.value ? 1 : 0);
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue*)value1;
                result = (double)val1.value + (val2.value ? 1 : 0);
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue*)value1;
                result = (double)val1.value + (val2.value ? 1 : 0);
            } else if (strcmp(val1Raw.valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue*)value1;
                result = (double)val1.value + (val2.value ? 1 : 0);
            } else if (strcmp(val1Raw.valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue*)value1;
                result = (double)val1.value + (val2.value ? 1 : 0);
            }
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue* resValue = malloc(sizeof(ShortValue));
                    short r = (short)result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    IntValue* resValue = malloc(sizeof(IntValue));
                    int r = (int)result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    LongValue* resValue = malloc(sizeof(LongValue));
                    long r = (long)result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    FloatValue* resValue = malloc(sizeof(FloatValue));
                    float r = (float)result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    DoubleValue* resValue = malloc(sizeof(DoubleValue));
                    double r = (double)result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                }
            }
        }
    }
    else if (strcmp(val1Raw.valueType, "string") == 0) {
        StringValue val1 = *(StringValue*)value1;
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            char* str2 = "";
            if (strcmp(val2Raw.valueType, "short") == 0) {
                ShortValue val2 = *(ShortValue*)value2;
                char str[(int)(ceil(log10(val2.value)+1)*sizeof(char))];
                sprintf(str, "%hd", val2.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val2Raw.valueType, "int") == 0) {
                IntValue val2 = *(IntValue*)value2;
                char str[(int)(ceil(log10(val2.value)+1)*sizeof(char))];
                sprintf(str, "%d", val2.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val2Raw.valueType, "long") == 0) {
                LongValue val2 = *(LongValue*)value2;
                char str[(int)(ceill(log10l(val2.value)+1)*sizeof(char))];
                sprintf(str, "%ld", val2.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val2Raw.valueType, "float") == 0) {
                FloatValue val2 = *(FloatValue*)value2;
                char str[(int)(ceilf(log10f(val2.value)+1)*sizeof(char))];
                sprintf(str, "%f", val2.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            } else if (strcmp(val2Raw.valueType, "double") == 0) {
                DoubleValue val2 = *(DoubleValue*)value2;
                char str[(int)(ceil(log10(val2.value)+1)*sizeof(char))];
                sprintf(str, "%f", val2.value);
                str2 = malloc(strlen(str));
                strcpy(str2, str);
            }
            StringValue* resValue = malloc(sizeof(StringValue));
            char* dest = concat(val1.value, str2);
            resValue->value = dest;
            resValue->base.valueType = "string";
            return resValue;
        } else if (strcmp(val2Raw.valueType, "string") == 0) {
            StringValue val2 = *(StringValue*)value2;
            char* result = concat(val1.value, val2.value);
            StringValue* resValue = malloc(sizeof(StringValue));
            resValue->value = result;
            resValue->base.valueType = "string";
            return resValue;
        } else if (strcmp(val2Raw.valueType, "bool") == 0) {
            BoolValue val2 = *(BoolValue*)value2;
            char* boolStr = val2.value ? "true" : "false";
            char* result = concat(val1.value, boolStr);
            StringValue* resValue = malloc(sizeof(StringValue));
            resValue->value = result;
            resValue->base.valueType = "string";
            return resValue;
        }
    }
    else if (strcmp(val1Raw.valueType, "bool") == 0) {
        BoolValue val1 = *(BoolValue*)value1;
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            double result = 0;
            if (strcmp(val2Raw.valueType, "short") == 0) {
                ShortValue val2 = *(ShortValue*)value2;
                result = (double)(val1.value ? 1 : 0) + (double)val2.value;
            } else if (strcmp(val2Raw.valueType, "int") == 0) {
                IntValue val2 = *(IntValue*)value2;
                result = (double)(val1.value ? 1 : 0) + (double)val2.value;
            } else if (strcmp(val2Raw.valueType, "long") == 0) {
                LongValue val2 = *(LongValue*)value2;
                result = (double)(val1.value ? 1 : 0) + (double)val2.value;
            } else if (strcmp(val2Raw.valueType, "float") == 0) {
                FloatValue val2 = *(FloatValue*)value2;
                result = (double)(val1.value ? 1 : 0) + (double)val2.value;
            } else if (strcmp(val2Raw.valueType, "double") == 0) {
                DoubleValue val2 = *(DoubleValue*)value2;
                result = (double)(val1.value ? 1 : 0) + (double)val2.value;
            }
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    ShortValue* resValue = malloc(sizeof(ShortValue));
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    IntValue* resValue = malloc(sizeof(IntValue));
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    LongValue* resValue = malloc(sizeof(LongValue));
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    FloatValue* resValue = malloc(sizeof(FloatValue));
                    resValue->value = r;
                    resValue->base.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    DoubleValue* resValue = malloc(sizeof(DoubleValue));
                    resValue->value = r;
                    resValue->base.valueType = "double";
                    return resValue;
                }
            }
        } else if (strcmp(val2Raw.valueType, "string") == 0) {
            StringValue val2 = *(StringValue*)value2;
            char* result = concat(val1.value ? "true" : "false", val2.value);
            StringValue* resValue = malloc(sizeof(StringValue));
            resValue->value = result;
            resValue->base.valueType = "string";
            return resValue;
        } else if (strcmp(val2Raw.valueType, "bool") == 0) {
            BoolValue val2 = *(BoolValue*)value2;
            bool result = val1.value || val2.value;
            BoolValue* resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
}

void* subtract(void* value1, void* value2) {
    Value val1Raw = *(Value*)value1;
    Value val2Raw = *(Value*)value2;
    if (strcmp(val1Raw.valueType, "int") == 0 ||
        strcmp(val1Raw.valueType, "short") == 0 ||
        strcmp(val1Raw.valueType, "long") == 0 ||
        strcmp(val1Raw.valueType, "float") == 0 ||
        strcmp(val1Raw.valueType, "double") == 0) {
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            long double result = 0;
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value - (long double) val2.value;
                }
            }
            if (floorl(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue *resValue = malloc(sizeof(ShortValue));
                    short r = (short) result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                } else if (result < INT32_MAX) {
                    IntValue *resValue = malloc(sizeof(IntValue));
                    int r = (int) result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                } else if (result < INT64_MAX) {
                    LongValue *resValue = malloc(sizeof(LongValue));
                    long r = (long) result;
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            } else {
                if (result < FLT_MAX) {
                    FloatValue *resValue = malloc(sizeof(FloatValue));
                    float r = (float) result;
                    resValue->value = r;
                    resValue->base.valueType = "float";
                    return resValue;
                } else if (result < DBL_MAX) {
                    DoubleValue *resValue = malloc(sizeof(DoubleValue));
                    double r = (double) result;
                    resValue->value = r;
                    resValue->base.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

void* multiply(void* value1, void* value2) {
    Value val1Raw = *(Value*)value1;
    Value val2Raw = *(Value*)value2;
    if (strcmp(val1Raw.valueType, "int") == 0 ||
        strcmp(val1Raw.valueType, "short") == 0 ||
        strcmp(val1Raw.valueType, "long") == 0 ||
        strcmp(val1Raw.valueType, "float") == 0 ||
        strcmp(val1Raw.valueType, "double") == 0) {
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            long double result = 0;
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value * (long double) val2.value;
                }
            }
            if (floorl(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue *resValue = malloc(sizeof(ShortValue));
                    short r = (short) result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                } else if (result < INT32_MAX) {
                    IntValue *resValue = malloc(sizeof(IntValue));
                    int r = (int) result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                } else if (result < INT64_MAX) {
                    LongValue *resValue = malloc(sizeof(LongValue));
                    long r = (long) result;
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            } else {
                if (result < FLT_MAX) {
                    FloatValue *resValue = malloc(sizeof(FloatValue));
                    float r = (float) result;
                    resValue->value = r;
                    resValue->base.valueType = "float";
                    return resValue;
                } else if (result < DBL_MAX) {
                    DoubleValue *resValue = malloc(sizeof(DoubleValue));
                    double r = (double) result;
                    resValue->value = r;
                    resValue->base.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

void* divide(void* value1, void* value2) {
    Value val1Raw = *(Value*)value1;
    Value val2Raw = *(Value*)value2;
    if (strcmp(val1Raw.valueType, "int") == 0 ||
        strcmp(val1Raw.valueType, "short") == 0 ||
        strcmp(val1Raw.valueType, "long") == 0 ||
        strcmp(val1Raw.valueType, "float") == 0 ||
        strcmp(val1Raw.valueType, "double") == 0) {
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            long double result = 0;
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                } else if (strcmp(val2Raw.valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value / (long double) val2.value;
                }
            }
            if (floorl(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue *resValue = malloc(sizeof(ShortValue));
                    short r = (short) result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                } else if (result < INT32_MAX) {
                    IntValue *resValue = malloc(sizeof(IntValue));
                    int r = (int) result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                } else if (result < INT64_MAX) {
                    LongValue *resValue = malloc(sizeof(LongValue));
                    long r = (long) result;
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            } else {
                if (result < FLT_MAX) {
                    FloatValue *resValue = malloc(sizeof(FloatValue));
                    float r = (float) result;
                    resValue->value = r;
                    resValue->base.valueType = "float";
                    return resValue;
                } else if (result < DBL_MAX) {
                    DoubleValue *resValue = malloc(sizeof(DoubleValue));
                    double r = (double) result;
                    resValue->value = r;
                    resValue->base.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

void* mod(void* value1, void* value2) {
    Value val1Raw = *(Value*)value1;
    Value val2Raw = *(Value*)value2;
    if (strcmp(val1Raw.valueType, "int") == 0 ||
        strcmp(val1Raw.valueType, "short") == 0 ||
        strcmp(val1Raw.valueType, "long") == 0 ||
        strcmp(val1Raw.valueType, "float") == 0 ||
        strcmp(val1Raw.valueType, "double") == 0) {
        if (strcmp(val2Raw.valueType, "int") == 0 ||
            strcmp(val2Raw.valueType, "short") == 0 ||
            strcmp(val2Raw.valueType, "long") == 0 ||
            strcmp(val2Raw.valueType, "float") == 0 ||
            strcmp(val2Raw.valueType, "double") == 0) {
            long double result = 0;
            if (strcmp(val1Raw.valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long) val1.value % (long) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                }
            } else if (strcmp(val1Raw.valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw.valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                } else if (strcmp(val2Raw.valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                } else if (strcmp(val2Raw.valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long) val1.value % (long) val2.value;
                }
            }
            if (floorl(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue *resValue = malloc(sizeof(ShortValue));
                    short r = (short) result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                } else if (result < INT32_MAX) {
                    IntValue *resValue = malloc(sizeof(IntValue));
                    int r = (int) result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                } else if (result < INT64_MAX) {
                    LongValue *resValue = malloc(sizeof(LongValue));
                    long r = (long) result;
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            }
        }
    }
}

void* power(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "double") == 0) {
            long double result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                }
            } else if (strcmp(val1Raw->valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = powl((long double) val1.value, (long double) val2.value);
                }
            }
            if (floorl(result) == result) {
                if (result < INT16_MAX) {
                    ShortValue *resValue = malloc(sizeof(ShortValue));
                    short r = (short) result;
                    resValue->value = r;
                    resValue->base.valueType = "short";
                    return resValue;
                } else if (result < INT32_MAX) {
                    IntValue *resValue = malloc(sizeof(IntValue));
                    int r = (int) result;
                    resValue->value = r;
                    resValue->base.valueType = "int";
                    return resValue;
                } else if (result < INT64_MAX) {
                    LongValue *resValue = malloc(sizeof(LongValue));
                    long r = (long) result;
                    resValue->value = r;
                    resValue->base.valueType = "long";
                    return resValue;
                }
            } else {
                if (result < FLT_MAX) {
                    FloatValue *resValue = malloc(sizeof(FloatValue));
                    float r = (float) result;
                    resValue->value = r;
                    resValue->base.valueType = "float";
                    return resValue;
                } else if (result < DBL_MAX) {
                    DoubleValue *resValue = malloc(sizeof(DoubleValue));
                    double r = (double) result;
                    resValue->value = r;
                    resValue->base.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

void* and(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value & val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
}

void* or(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value | val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
}

void* equal(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "double") == 0) {
            bool result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) *(long *) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) *(long *) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) *(long *) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) *(long *) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) *(long *) val1.value == (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value == (long double) val2.value;
                }
            }
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "string") == 0) {
        StringValue* val1 = (StringValue*) value1;
        if (strcmp(val2Raw->valueType, "string") == 0) {
            StringValue* val2 = (StringValue*) value2;
            bool result = strcmp(val1->value, val2->value) == 0;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value == val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    BoolValue *resValue = malloc(sizeof(BoolValue));
    resValue->value = false;
    resValue->base.valueType = "bool";
    return resValue;
}

void* ne(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "double") == 0) {
            bool result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) *(long *) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) *(long *) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) *(long *) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) *(long *) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) *(long *) val1.value != (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value != (long double) val2.value;
                }
            }
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "string") == 0) {
        StringValue* val1 = (StringValue*) value1;
        if (strcmp(val2Raw->valueType, "string") == 0) {
            StringValue* val2 = (StringValue*) value2;
            bool result = strcmp(val1->value, val2->value) != 0;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value != val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    BoolValue *resValue = malloc(sizeof(BoolValue));
    resValue->value = false;
    resValue->base.valueType = "bool";
    return resValue;
}

void* lt(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "long double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "long double") == 0) {
            bool result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) *(long *) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) *(long *) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) *(long *) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) *(long *) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) *(long *) val1.value < (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value < (long double) val2.value;
                }
            }
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "string") == 0) {
        StringValue* val1 = (StringValue*) value1;
        if (strcmp(val2Raw->valueType, "string") == 0) {
            StringValue* val2 = (StringValue*) value2;
            bool result = strcmp(val1->value, val2->value) < 0;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value < val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    BoolValue *resValue = malloc(sizeof(BoolValue));
    resValue->value = false;
    resValue->base.valueType = "bool";
    return resValue;
}

void* le(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "long double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "long double") == 0) {
            bool result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) *(long *) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) *(long *) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) *(long *) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) *(long *) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) *(long *) val1.value <= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value <= (long double) val2.value;
                }
            }
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "string") == 0) {
        StringValue* val1 = (StringValue*) value1;
        if (strcmp(val2Raw->valueType, "string") == 0) {
            StringValue* val2 = (StringValue*) value2;
            bool result = strcmp(val1->value, val2->value) <= 0;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value <= val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    BoolValue *resValue = malloc(sizeof(BoolValue));
    resValue->value = false;
    resValue->base.valueType = "bool";
    return resValue;
}

void* ge(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "long double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "long double") == 0) {
            bool result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) *(long *) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) *(long *) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) *(long *) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) *(long *) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) *(long *) val1.value >= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value >= (long double) val2.value;
                }
            }
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "string") == 0) {
        StringValue* val1 = (StringValue*) value1;
        if (strcmp(val2Raw->valueType, "string") == 0) {
            StringValue* val2 = (StringValue*) value2;
            bool result = strcmp(val1->value, val2->value) >= 0;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value >= val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    BoolValue *resValue = malloc(sizeof(BoolValue));
    resValue->value = false;
    resValue->base.valueType = "bool";
    return resValue;
}

void* gt(void* value1, void* value2) {
    Value* val1Raw = (Value*)value1;
    Value* val2Raw = (Value*)value2;
    if (strcmp(val1Raw->valueType, "int") == 0 ||
        strcmp(val1Raw->valueType, "short") == 0 ||
        strcmp(val1Raw->valueType, "long") == 0 ||
        strcmp(val1Raw->valueType, "float") == 0 ||
        strcmp(val1Raw->valueType, "long double") == 0) {
        if (strcmp(val2Raw->valueType, "int") == 0 ||
            strcmp(val2Raw->valueType, "short") == 0 ||
            strcmp(val2Raw->valueType, "long") == 0 ||
            strcmp(val2Raw->valueType, "float") == 0 ||
            strcmp(val2Raw->valueType, "long double") == 0) {
            bool result = false;
            if (strcmp(val1Raw->valueType, "short") == 0) {
                ShortValue val1 = *(ShortValue *) value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "int") == 0) {
                IntValue val1 = *(IntValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long") == 0) {
                LongValue val1 = *(LongValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) *(long *) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) *(long *) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) *(long *) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) *(long *) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) *(long *) val1.value > (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "float") == 0) {
                FloatValue val1 = *(FloatValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                }
            } else if (strcmp(val1Raw->valueType, "long double") == 0) {
                DoubleValue val1 = *(DoubleValue *) &value1;
                if (strcmp(val2Raw->valueType, "short") == 0) {
                    ShortValue val2 = *(ShortValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "int") == 0) {
                    IntValue val2 = *(IntValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long") == 0) {
                    LongValue val2 = *(LongValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "float") == 0) {
                    FloatValue val2 = *(FloatValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                } else if (strcmp(val2Raw->valueType, "long double") == 0) {
                    DoubleValue val2 = *(DoubleValue *) &value2;
                    result = (long double) val1.value > (long double) val2.value;
                }
            }
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "string") == 0) {
        StringValue* val1 = (StringValue*) value1;
        if (strcmp(val2Raw->valueType, "string") == 0) {
            StringValue* val2 = (StringValue*) value2;
            bool result = strcmp(val1->value, val2->value) > 0;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    else if (strcmp(val1Raw->valueType, "bool") == 0) {
        BoolValue* val1 = (BoolValue*) value1;
        if (strcmp(val2Raw->valueType, "bool") == 0) {
            BoolValue* val2 = (BoolValue*) value2;
            bool result = val1->value > val2->value;
            BoolValue *resValue = malloc(sizeof(BoolValue));
            resValue->value = result;
            resValue->base.valueType = "bool";
            return resValue;
        }
    }
    BoolValue *resValue = malloc(sizeof(BoolValue));
    resValue->value = false;
    resValue->base.valueType = "bool";
    return resValue;
}

int convertBytesToInt(const char bytes[]) {
    return ((bytes[3] & 0xff)) | ((bytes[2] & 0xff) << 8) | ((bytes[1] & 0xff) << 16) | ((bytes[0] & 0xff) << 24);
}

short convertBytesToShort(const char* bytes) {
    return ((bytes[0] & 0xff) << 8) | ((bytes[1] & 0xff) << 0);
}

unsigned long calculateBytes(int investigateId, char* c, unsigned long p) {
    while (!expStack.isEmpty(&expStack)) {
        if (c[p] == 0x7f) {
            p++;
            char chainLengthBytes[4];
            for (int index = 0; index < 4; index++)
                chainLengthBytes[index] = c[p + index];
            p += 4;
            int chainLength = convertBytesToInt(chainLengthBytes);
            struct Reference* firstChain = NULL;
            struct Reference* ref = NULL;
            for (int i = 0; i < chainLength; i++) {
                char idLengthBytes[4];
                for (int index = 0; index < 4; index++)
                    idLengthBytes[index] = c[p + index];
                p += 4;
                int idLength = convertBytesToInt(idLengthBytes);
                char idName[idLength];
                for (int index = 0; index < idLength; index++)
                    idName[index] = c[p + index];
                p += idLength;
                Identifier* id = malloc(sizeof(Identifier));
                id->id = malloc(strlen(idName) + 1);
                strcpy(id->id, idName);
                id->exp.type = "Identifier";
                id->exp.base.type = "Identifier";
                struct Reference* newRef = malloc(sizeof(struct Reference));
                newRef->currentChain = id;
                newRef->base.type = "Reference";
                if (ref != NULL)
                    ref->restOfTheChain = newRef;
                else
                    firstChain = newRef;
                ref = newRef;
            }
            if (investigateId == 1) {
                void* objectChain = NULL;
                struct StackDataItem *iterator = dataStack.item;
                char* chainName = malloc(strlen(firstChain->currentChain->id) + 1);
                strcpy(chainName, firstChain->currentChain->id);
                void* target = dict_get((Dictionary *) iterator->data, chainName);
                while (target == NULL && iterator->prev != NULL) {
                    iterator = iterator->prev;
                    target = dict_get((Dictionary *) iterator->data, firstChain->currentChain->id);
                }
                if (target != NULL && strcmp(((Code*)target)->type, "Object") == 0)
                    objectChain = target;
                firstChain = firstChain->restOfTheChain;
                char* refStr = chainName;
                while (target != NULL && firstChain != NULL) {
                    if (strcmp(((Code*)target)->type, "Object") == 0) {
                        chainName = malloc(strlen(firstChain->currentChain->id) + 1);
                        strcpy(chainName, firstChain->currentChain->id);
                        void* target2 = dict_get(((Object*)target)->value, chainName);
                        if (target2 != NULL)
                            target = target2;
                        else {
                            target2 = dict_get(((Object*)target)->funcs, chainName);
                            if (target2 != NULL)
                                target = target2;
                        }
                    }
                    else if (strcmp(((Code*)target)->type, "Class") == 0) {
                        chainName = malloc(strlen(firstChain->currentChain->id) + 1);
                        strcpy(chainName, firstChain->currentChain->id);
                        target = dict_get(((Class*)target)->functions, chainName);
                    }
                    else if (strcmp(((Code*)target)->type, "Function") == 0) {
                        chainName = malloc(strlen(firstChain->currentChain->id) + 1);
                        strcpy(chainName, firstChain->currentChain->id);
                        strcat(refStr, ".");
                        strcat(refStr, chainName);
                        dict_add((Dictionary*) expStack.top(&expStack), "value", target);
                        dict_add((Dictionary*) expStack.top(&expStack), "value2", refStr);
                        dict_add((Dictionary*) expStack.top(&expStack), "value3", objectChain);
                        return p;
                    }
                    strcat(refStr, ".");
                    strcat(refStr, firstChain->currentChain->id);
                    firstChain = firstChain->restOfTheChain;
                    if (target != NULL && strcmp(((Code*)target)->type, "Object") == 0)
                        objectChain = target;
                }
                dict_add((Dictionary*) expStack.top(&expStack), "value", target);
                dict_add((Dictionary*) expStack.top(&expStack), "value2", refStr);
                dict_add((Dictionary*) expStack.top(&expStack), "value3", objectChain);
            } else {
                dict_add((Dictionary*) expStack.top(&expStack), "value", firstChain);
            }
            return p;
        }
        else if (c[p] == 0x55) {
            p++;
            if (c[p] == 0x01) {
                p++;
                Dictionary* expDict = dict_new();
                expStack.push(&expStack, expDict);
                int tempMachineState = machineState;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* target = dict_get((Dictionary*)expStack.top(&expStack), "value");
                char* refStr = dict_get((Dictionary*)expStack.top(&expStack), "value2");
                Object* thisObj = dict_get((Dictionary*)expStack.pop(&expStack), "value3");
                if (c[p] == 0x02) {
                    p++;
                    char entriesCountBytes[4];
                    for (int index = 0; index < 4; index++)
                        entriesCountBytes[index] = c[p + index];
                    p += 4;
                    int entriesCount = convertBytesToInt(entriesCountBytes);
                    Dictionary* entriesDict = dict_new();
                    for (int counter = 0; counter < entriesCount; counter++) {
                        if (c[p] == 0x03) {
                            p++;
                            char keyLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                keyLengthBytes[index] = c[p + index];
                            p += 4;
                            int keyLength = convertBytesToInt(keyLengthBytes);
                            char keyBytes[keyLength];
                            for (int index = 0; index < keyLength; index++)
                                keyBytes[index] = c[p + index];
                            p += keyLength;
                            char *key = &keyBytes[0];
                            char valueLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                valueLengthBytes[index] = c[p + index];
                            p += 4;
                            int valueLength = convertBytesToInt(valueLengthBytes);
                            Dictionary *expD = dict_new();
                            expStack.push(&expStack, expD);
                            p = calculateBytes(1, c, p);
                            dict_add(entriesDict, key, dict_get((Dictionary *) expStack.pop(&expStack), "value"));
                        }
                    }
                    if (target == NULL) {
                        routeAndResolve(refStr, entriesDict);
                    } else {
                        Function* func = (Function*) target;
                        if (thisObj != NULL)
                            dict_add(entriesDict, "this", thisObj);
                        dict_add((Dictionary*)expStack.top(&expStack), "value", executeIntern(func->codes, func->loc, entriesDict));
                    }
                    return p;
                }
            }
        } else if (c[p] == 0x57) {
            p++;
            if (c[p] == 0x01) {
                p++;
                Dictionary* expDict = dict_new();
                expStack.push(&expStack, expDict);
                p = calculateBytes(1, c, p);
                void* target = dict_get((Dictionary*)expStack.pop(&expStack), "value");
                if (c[p] == 0x02) {
                    p++;
                    char entriesCountBytes[4];
                    for (int index = 0; index < 4; index++)
                        entriesCountBytes[index] = c[p + index];
                    p += 4;
                    int entriesCount = convertBytesToInt(entriesCountBytes);
                    Dictionary* entriesDict = dict_new();
                    for (int counter = 0; counter < entriesCount; counter++) {
                        if (c[p] == 0x03) {
                            p++;
                            char keyLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                keyLengthBytes[index] = c[p + index];
                            p += 4;
                            int keyLength = convertBytesToInt(keyLengthBytes);
                            char keyBytes[keyLength];
                            for (int index = 0; index < keyLength; index++)
                                keyBytes[index] = c[p + index];
                            p += keyLength;
                            char *key = &keyBytes[0];
                            char valueLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                valueLengthBytes[index] = c[p + index];
                            p += 4;
                            int valueLength = convertBytesToInt(valueLengthBytes);
                            Dictionary *expD = dict_new();
                            expStack.push(&expStack, expD);
                            p = calculateBytes(1, c, p);
                            dict_add(entriesDict, key, dict_get((Dictionary *) expStack.pop(&expStack), "value"));
                        }
                    }
                    if (target != NULL) {
                        Class* classObj = (Class*) target;
                        Object* object = malloc(sizeof(Object));
                        object->base.type = "Object";
                        object->value = dict_new();
                        while (classObj->properties->iteratorHasNext(classObj->properties)) {
                            Prop* prop = (Prop*) classObj->properties->iteratorForward(classObj->properties);
                            Dictionary* dict = dict_new();
                            expStack.push(&expStack, dict);
                            calculateBytes(1, prop->value, 0);
                            char* propId = malloc(strlen(prop->id->id) + 1);
                            strcpy(propId, prop->id->id);
                            dict_add(object->value, propId, dict_get((Dictionary*)expStack.pop(&expStack), "value"));
                        }
                        object->funcs = classObj->functions;
                        dict_add((Dictionary*)expStack.top(&expStack), "value", object);
                    }
                    return p;
                }
            }
        } else if (c[p] == 0x71) {
            p++;
            if (c[p] == 0x01) {
                p++;
                expStack.push(&expStack, dict_new());
                p = calculateBytes(1, c, p);
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    expStack.push(&expStack, dict_new());
                    p = calculateBytes(1, c, p);
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = sum(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x72) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = subtract(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x73) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = multiply(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x74) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = divide(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x75) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = mod(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x76) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = power(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x77) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = and(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x78) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = or(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x79) {
            p++;
            if (c[p] == 0x01) {
                p++;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                p = calculateBytes(1, c, p);
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    p = calculateBytes(1, c, p);
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = equal(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x7a) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = gt(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x7b) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = ge(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x7c) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = ne(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x7d) {
            machineState = c[p];
            p++;
            if (c[p] == 0x01) {
                p++;
                machineState = 0x711;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                int tempMachineState = machineState;
                machineState = 0x00;
                p = calculateBytes(1, c, p);
                machineState = tempMachineState;
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    tempMachineState = machineState;
                    machineState = 0x00;
                    p = calculateBytes(1, c, p);
                    machineState = tempMachineState;
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = le(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        } else if (c[p] == 0x7e) {
            p++;
            if (c[p] == 0x01) {
                p++;
                Dictionary* dict = dict_new();

                expStack.push(&expStack, dict);
                p = calculateBytes(1, c, p);
                void* value1 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                if (c[p] == 0x02) {
                    p++;
                    Dictionary* dict2 = dict_new();

                    expStack.push(&expStack, dict2);
                    p = calculateBytes(1, c, p);
                    void* value2 = dict_get((Dictionary*) expStack.pop(&expStack),"value");
                    void* result = lt(value1, value2);
                    dict_add((Dictionary*) expStack.top(&expStack),"value", result);
                    return p;
                }
            }
        }
        else if (c[p] == 0x61) {
            p++;
            char idNameLengthArr[4];
            for (int index = 0; index < (int)sizeof(idNameLengthArr); index++)
                idNameLengthArr[index] = c[p + index];
            p += 4;
            int idNameLength = convertBytesToInt(idNameLengthArr);
            char idNameArr[idNameLength];
            for (int index = 0; index < idNameLength; index++)
                idNameArr[index] = c[p + index];
            p += idNameLength;
            Identifier* id = malloc(sizeof(Identifier));
            id->id = malloc(strlen(idNameArr) + 1);
            strcpy(id->id, idNameArr);
            id->exp.type = "Identifier";
            id->exp.base.type = "Identifier";
            if (investigateId == 1) {
                struct StackDataItem *iterator = dataStack.item;
                char* idName = malloc(strlen(id->id) + 1);
                strcpy(idName, id->id);
                void *value = dict_get((Dictionary *) iterator->data, idName);
                while (value == NULL && iterator->prev != NULL) {
                    iterator = iterator->prev;
                    value = dict_get((Dictionary *) iterator->data, idName);
                }
                if (value == NULL)
                    dict_add((Dictionary *) expStack.top(&expStack),"value", id);
                else
                    dict_add((Dictionary *) expStack.top(&expStack),"value", value);
            }
            else {
                dict_add((Dictionary *) expStack.top(&expStack),"value", id);
            }
            return p;
        } else if (c[p] == 0x62) {
            p++;
            char valueLengthArr[4];
            for (int index = 0; index < (int)sizeof(valueLengthArr); index++)
                valueLengthArr[index] = c[p + index];
            p += (int)sizeof(valueLengthArr);
            int valueLength = convertBytesToInt(valueLengthArr);
            char valueArr[valueLength];
            for (int index = 0; index < valueLength; index++)
                valueArr[index] = c[p + index];
            p += valueLength;
            StringValue* val = malloc(sizeof(StringValue));
            val->value = malloc(strlen(valueArr)+1);
            strcpy(val->value, valueArr);
            val->base.valueType = "string";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*)expStack.top(&expStack), "value", val);
            return p;
        } else if (c[p] == 0x63) {
            p++;
            char valueArr[8];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = c[p + index];
            p += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            DoubleValue* val = malloc(sizeof(DoubleValue));
            val->value = value;
            val->base.valueType = "double";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*)expStack.top(&expStack), "value", val);
            return p;
        } else if (c[p] == 0x64) {
            p++;
            char valueArr[4];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = c[p + index];
            p += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            float value;
            memcpy(&value, valueRaw, sizeof(float));
            FloatValue* val = malloc(sizeof(FloatValue));
            val->value = value;
            val->base.valueType = "float";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*)expStack.top(&expStack), "value", val);
            return p;
        } else if (c[p] == 0x65) {
            machineState = c[p];
            p++;
            char valueArr[2];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = c[p + index];
            p += (int)sizeof(valueArr);
            ShortValue* val = malloc(sizeof(ShortValue));
            val->value = convertBytesToShort(valueArr);;
            val->base.valueType = "short";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*)expStack.top(&expStack), "value", val);
            return p;
        } else if (c[p] == 0x66) {
            p++;
            char valueArr[4];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = c[p + index];
            p += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            int value;
            memcpy(&value, valueRaw, sizeof(int));
            IntValue* val = malloc(sizeof(IntValue));
            val->value = value;
            val->base.valueType = "integer";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*)expStack.top(&expStack), "value", val);
            return p;
        } else if (c[p] == 0x67) {
            p++;
            char valueArr[8];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = c[p + index];
            p += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            long value;
            memcpy(&value, valueRaw, sizeof(long));
            LongValue* val = malloc(sizeof(LongValue));
            val->value = value;
            val->base.valueType = "long";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*) expStack.top(&expStack),"value", val);
            return p;
        } else if (c[p] == 0x68) {
            p++;
            char valueArr[1];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = c[p + index];
            p += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            bool value;
            memcpy(&value, valueRaw, sizeof(bool));
            BoolValue* val = malloc(sizeof(BoolValue));
            val->value = value;
            val->base.valueType = "boolean";
            val->base.exp.type = "Value";
            val->base.exp.base.type = "Value";
            dict_add((Dictionary*) expStack.top(&expStack),"value", val);
            return p;
        }
    }
}

void calculate(int investigateId) {
    pointer = calculateBytes(investigateId, code, pointer);
}

void* ride() {
    while (codeLength > pointer) {
        if (code[pointer] == 0x6e) {
            break;
        }
        if (code[pointer] == 0x51) {
            pointer++;
            Function* function = malloc(sizeof(Function));
            function->base.type = "Function";
            if (code[pointer] == 0x01) {
                pointer++;
                char funcNameLengthBytes[4];
                for (int index = 0; index < (int)sizeof(funcNameLengthBytes); index++)
                    funcNameLengthBytes[index] = code[pointer + index];
                pointer += (int)sizeof(funcNameLengthBytes);
                int funcNameLength = convertBytesToInt(funcNameLengthBytes);
                char funcName[funcNameLength];
                for (int index = 0; index < funcNameLength; index++)
                    funcName[index] = code[pointer + index];
                pointer += funcNameLength;
                char* funcNameStr = malloc(sizeof(funcName));
                strcpy(funcNameStr, funcName);
                if (code[pointer] == 0x02) {
                    pointer++;
                    char funcLevelLengthBytes[4];
                    for (int index = 0; index < (int)sizeof(funcLevelLengthBytes); index++)
                        funcLevelLengthBytes[index] = code[pointer + index];
                    pointer += (int)sizeof(funcLevelLengthBytes);
                    int funcLevelLength = convertBytesToInt(funcLevelLengthBytes);
                    char funcLevelStr[funcLevelLength];
                    for (int index = 0; index < funcLevelLength; index++)
                        funcLevelStr[index] = code[pointer + index];
                    pointer += funcLevelLength;
                    if (code[pointer] == 0x03) {
                        pointer++;
                        struct List* identifiers = malloc(sizeof(struct List));
                        initList(identifiers);
                        char paramsCountBytes[4];
                        for (int index = 0; index < 4; index++)
                            paramsCountBytes[index] = code[pointer + index];
                        pointer += 4;
                        int paramsCount = convertBytesToInt(paramsCountBytes);
                        for (int counter = 0; counter < paramsCount; counter++) {
                            char paramLengthBytes[4];
                            for (int index = 0; index < (int)sizeof(paramLengthBytes); index++)
                                paramLengthBytes[index] = code[pointer + index];
                            pointer += (int)sizeof(paramLengthBytes);
                            int paramLength = convertBytesToInt(paramLengthBytes);
                            pointer += paramLength;
                            char idStr[paramLength];
                            for (int index = 0; index < paramLength; index++)
                                idStr[index] = code[pointer + index];
                            pointer += paramLength;
                            char* id = &idStr[0];
                            identifiers->append(identifiers, id);
                        }
                        if (code[pointer] == 0x04) {
                            pointer++;
                            if (code[pointer] == 0x6f) {
                                pointer++;
                                char jumpBytes[4];
                                for (int index = 0; index < (int)sizeof(jumpBytes); index++)
                                    jumpBytes[index] = code[pointer + index];
                                pointer += (int)sizeof(jumpBytes);
                                int jump = convertBytesToInt(jumpBytes);
                                char body[jump];
                                for(unsigned long i = pointer; i < pointer + jump; i++)
                                    body[i - pointer] = code[i];
                                pointer += jump;
                                if (code[pointer] == 0x6e) {
                                    pointer++;
                                    function->base.type = "Function";
                                    function->funcName = funcNameStr;
                                    function->params = identifiers;
                                    function->codes = malloc(jump);
                                    memcpy(function->codes, body, jump);
                                    function->loc = jump;
                                    dict_add((Dictionary *) dataStack.top(&dataStack), funcNameStr, function);
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (code[pointer] == 0x57) {
            pointer++;
            if (code[pointer] == 0x01) {
                pointer++;
                Dictionary* expDict = dict_new();
                expStack.push(&expStack, expDict);
                calculate(1);
                void* target = dict_get((Dictionary*)expStack.pop(&expStack), "value");
                if (code[pointer] == 0x02) {
                    pointer++;
                    char entriesCountBytes[4];
                    for (int index = 0; index < 4; index++)
                        entriesCountBytes[index] = code[pointer + index];
                    pointer += 4;
                    int entriesCount = convertBytesToInt(entriesCountBytes);
                    Dictionary* entriesDict = dict_new();
                    for (int counter = 0; counter < entriesCount; counter++) {
                        if (code[pointer] == 0x03) {
                            pointer++;
                            char keyLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                keyLengthBytes[index] = code[pointer + index];
                            pointer += 4;
                            int keyLength = convertBytesToInt(keyLengthBytes);
                            char keyBytes[keyLength];
                            for (int index = 0; index < keyLength; index++)
                                keyBytes[index] = code[pointer + index];
                            pointer += keyLength;
                            char *key = &keyBytes[0];
                            char valueLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                valueLengthBytes[index] = code[pointer + index];
                            pointer += 4;
                            int valueLength = convertBytesToInt(valueLengthBytes);
                            Dictionary *expD = dict_new();
                            expStack.push(&expStack, expD);
                            calculate(1);
                            dict_add(entriesDict, key, dict_get((Dictionary *) expStack.pop(&expStack), "value"));
                        }
                    }
                    if (target != NULL) {
                        Class* classObj = (Class*) target;
                        Object* object = malloc(sizeof(Object));
                        object->base.type = "Object";
                        object->value = dict_new();
                        while (classObj->properties->iteratorHasNext(classObj->properties)) {
                            Prop* prop = (Prop*) classObj->properties->iteratorForward(classObj->properties);
                            Dictionary* dict = dict_new();
                            expStack.push(&expStack, dict);
                            calculate(1);
                            char* propId = malloc(strlen(prop->id->id) + 1);
                            strcpy(propId, prop->id->id);
                            dict_add(object->value, propId, dict_get((Dictionary*)expStack.pop(&expStack), "value"));
                        }
                        object->funcs = classObj->functions;
                    }
                }
            }
        }
        else if (code[pointer] == 0x58) {
            pointer++;
            Class* classObj = malloc(sizeof(Class));
            classObj->base.type = "Class";
            if (code[pointer] == 0x01) {
                pointer++;
                char classNameLengthBytes[4];
                for (int index = 0; index < (int)sizeof(classNameLengthBytes); index++)
                    classNameLengthBytes[index] = code[pointer + index];
                pointer += (int)sizeof(classNameLengthBytes);
                int classNameLength = convertBytesToInt(classNameLengthBytes);
                char className[classNameLength];
                for (int index = 0; index < classNameLength; index++)
                    className[index] = code[pointer + index];
                pointer += classNameLength;
                char* classNameStr = malloc(sizeof(className));
                strcpy(classNameStr, className);
                if (code[pointer] == 0x02) {
                    pointer++;
                    char inheritanceCountBytes[4];
                    for (int index = 0; index < (int)sizeof(inheritanceCountBytes); index++)
                        inheritanceCountBytes[index] = code[pointer + index];
                    pointer += (int)sizeof(inheritanceCountBytes);
                    int inheritanceCount = convertBytesToInt(inheritanceCountBytes);
                    struct List* inheritance = malloc(sizeof(struct List));
                    initList(inheritance);
                    for (int i = 0; i < inheritanceCount; i++) {
                        char identifierNameLengthBytes[4];
                        for (int index = 0; index < (int)sizeof(identifierNameLengthBytes); index++)
                            identifierNameLengthBytes[index] = code[pointer + index];
                        pointer += (int)sizeof(identifierNameLengthBytes);
                        int idNameLength = convertBytesToInt(identifierNameLengthBytes);
                        char idNameBytes[idNameLength];
                        for (int index = 0; index < idNameLength; index++)
                            idNameBytes[index] = code[pointer + index];
                        pointer += idNameLength;
                        char* idName = malloc(strlen(idNameBytes) + 1);
                        strcpy(idName, idNameBytes);
                        Identifier* id = malloc(sizeof(Identifier));
                        id->id = idName;
                        inheritance->append(inheritance, id);
                    }
                    if (code[pointer] == 0x03) {
                        pointer++;
                        char behaviorCountBytes[4];
                        for (int index = 0; index < (int)sizeof(behaviorCountBytes); index++)
                            behaviorCountBytes[index] = code[pointer + index];
                        pointer += (int)sizeof(behaviorCountBytes);
                        int behaviorCount = convertBytesToInt(behaviorCountBytes);
                        struct List* behavior = malloc(sizeof(struct List));
                        initList(behavior);
                        for (int i = 0; i < behaviorCount; i++) {
                            char identifierNameLengthBytes[4];
                            for (int index = 0; index < (int)sizeof(identifierNameLengthBytes); index++)
                                identifierNameLengthBytes[index] = code[pointer + index];
                            pointer += (int)sizeof(identifierNameLengthBytes);
                            int idNameLength = convertBytesToInt(identifierNameLengthBytes);
                            char idNameBytes[idNameLength];
                            for (int index = 0; index < idNameLength; index++)
                                idNameBytes[index] = code[pointer + index];
                            pointer += idNameLength;
                            char* idName = &idNameBytes[0];
                            Identifier* id = malloc(sizeof(Identifier));
                            id->id = idName;
                            behavior->append(behavior, id);
                        }
                        if (code[pointer] == 0x04) {
                            pointer++;
                            char propCountBytes[4];
                            for (int index = 0; index < (int)sizeof(propCountBytes); index++)
                                propCountBytes[index] = code[pointer + index];
                            pointer += (int)sizeof(propCountBytes);
                            int propCount = convertBytesToInt(propCountBytes);
                            struct List* props = malloc(sizeof(struct List));
                            initList(props);
                            for (int i = 0; i < propCount; i++) {
                                char identifierNameLengthBytes[4];
                                for (int index = 0; index < (int)sizeof(identifierNameLengthBytes); index++)
                                    identifierNameLengthBytes[index] = code[pointer + index];
                                pointer += (int)sizeof(identifierNameLengthBytes);
                                int idNameLength = convertBytesToInt(identifierNameLengthBytes);
                                char idNameBytes[idNameLength];
                                for (int index = 0; index < idNameLength; index++)
                                    idNameBytes[index] = code[pointer + index];
                                pointer += idNameLength;
                                char* idName = malloc(strlen(idNameBytes) + 1);
                                strcpy(idName, idNameBytes);
                                Identifier* id = malloc(sizeof(Identifier));
                                id->id = idName;
                                id->exp.type = "Identifier";
                                id->exp.base.type = "Identifier";
                                char valueLengthBytes[4];
                                for (int index = 0; index < (int)sizeof(valueLengthBytes); index++)
                                    valueLengthBytes[index] = code[pointer + index];
                                pointer += (int)sizeof(valueLengthBytes);
                                unsigned long valueLength = convertBytesToInt(valueLengthBytes);
                                char value[valueLength];
                                for (unsigned long i2 = pointer; i2 < pointer + valueLength; i2++)
                                    value[i2 - pointer] = code[i2];
                                pointer += valueLength;
                                Prop* prop = malloc(sizeof(Prop));
                                prop->id = id;
                                prop->value = malloc(valueLength);
                                memcpy(prop->value, value, valueLength);
                                prop->loc = valueLength;
                                props->append(props, prop);
                            }
                            if (code[pointer] == 0x05) {
                                pointer++;
                                char funcCountBytes[4];
                                for (int index = 0; index < (int)sizeof(funcCountBytes); index++)
                                    funcCountBytes[index] = code[pointer + index];
                                pointer += (int)sizeof(funcCountBytes);
                                int funcCount = convertBytesToInt(funcCountBytes);
                                Dictionary* funcs = dict_new();
                                for (int i = 0; i < funcCount; i++) {
                                    if (code[pointer] == 0x51) {
                                        pointer++;
                                        Function* function = malloc(sizeof(Function));
                                        function->base.type = "Function";
                                        if (code[pointer] == 0x01) {
                                            pointer++;
                                            char funcNameLengthBytes[4];
                                            for (int index = 0; index < (int)sizeof(funcNameLengthBytes); index++)
                                                funcNameLengthBytes[index] = code[pointer + index];
                                            pointer += (int)sizeof(funcNameLengthBytes);
                                            int funcNameLength = convertBytesToInt(funcNameLengthBytes);
                                            char funcName[funcNameLength];
                                            for (int index = 0; index < funcNameLength; index++)
                                                funcName[index] = code[pointer + index];
                                            pointer += funcNameLength;
                                            char* funcNameStr = malloc(sizeof(funcName));
                                            strcpy(funcNameStr, funcName);
                                            if (code[pointer] == 0x02) {
                                                pointer++;
                                                char funcLevelLengthBytes[4];
                                                for (int index = 0; index < (int)sizeof(funcLevelLengthBytes); index++)
                                                    funcLevelLengthBytes[index] = code[pointer + index];
                                                pointer += (int)sizeof(funcLevelLengthBytes);
                                                int funcLevelLength = convertBytesToInt(funcLevelLengthBytes);
                                                char funcLevelStr[funcLevelLength];
                                                for (int index = 0; index < funcLevelLength; index++)
                                                    funcLevelStr[index] = code[pointer + index];
                                                pointer += funcLevelLength;
                                                if (code[pointer] == 0x03) {
                                                    pointer++;
                                                    struct List* identifiers = malloc(sizeof(struct List));
                                                    initList(identifiers);
                                                    char paramsCountBytes[4];
                                                    for (int index = 0; index < 4; index++)
                                                        paramsCountBytes[index] = code[pointer + index];
                                                    pointer += 4;
                                                    int paramsCount = convertBytesToInt(paramsCountBytes);
                                                    for (int counter = 0; counter < paramsCount; counter++) {
                                                        char paramLengthBytes[4];
                                                        for (int index = 0; index < (int)sizeof(paramLengthBytes); index++)
                                                            paramLengthBytes[index] = code[pointer + index];
                                                        pointer += (int)sizeof(paramLengthBytes);
                                                        int paramLength = convertBytesToInt(paramLengthBytes);
                                                        pointer += paramLength;
                                                        char idStr[paramLength];
                                                        for (int index = 0; index < paramLength; index++)
                                                            idStr[index] = code[pointer + index];
                                                        pointer += paramLength;
                                                        char* id = &idStr[0];
                                                        identifiers->append(identifiers, id);
                                                    }
                                                    if (code[pointer] == 0x04) {
                                                        pointer++;
                                                        if (code[pointer] == 0x6f) {
                                                            pointer++;
                                                            char jumpBytes[4];
                                                            for (int index = 0; index < (int)sizeof(jumpBytes); index++)
                                                                jumpBytes[index] = code[pointer + index];
                                                            pointer += (int)sizeof(jumpBytes);
                                                            int jump = convertBytesToInt(jumpBytes);
                                                            char body[jump];
                                                            for(unsigned long i = pointer; i < pointer + jump; i++)
                                                                body[i - pointer] = code[i];
                                                            pointer += jump;
                                                            if (code[pointer] == 0x6e) {
                                                                pointer++;
                                                                function->base.type = "Function";
                                                                function->funcName = funcNameStr;
                                                                function->params = identifiers;
                                                                function->codes = malloc(jump);
                                                                memcpy(function->codes, body, jump);
                                                                function->loc = jump;
                                                                dict_add(funcs, funcNameStr, function);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                classObj->inheritance = inheritance;
                                classObj->behavior = behavior;
                                classObj->properties = props;
                                classObj->functions = funcs;
                                classObj->className = className;
                                dict_add((Dictionary *) dataStack.top(&dataStack), className, classObj);
                            }
                        }
                    }
                }
            }
        }
        else if (code[pointer] == 0x52) {
            pointer++;
            bool matched = false;
            if (code[pointer] == 0x01) {
                pointer++;
                Dictionary* expDict = dict_new();
                expStack.push(&expStack, expDict);
                int tempMachineState = machineState;
                calculate(1);
                machineState = tempMachineState;
                BoolValue* condition = (BoolValue*)dict_get((Dictionary*)expStack.pop(&expStack), "value");
                if (code[pointer] == 0x02) {
                    pointer++;
                    if (code[pointer] == 0x6f) {
                        pointer++;
                        char jumpBytes[4];
                        for (int index = 0; index < (int)sizeof(jumpBytes); index++)
                            jumpBytes[index] = code[pointer + index];
                        pointer += (int)sizeof(jumpBytes);
                        int jump = convertBytesToInt(jumpBytes);
                        char body[jump];
                        for(unsigned long i = pointer; i < pointer + jump; i++)
                            body[i - pointer] = code[i];
                        pointer += jump;
                        if (condition->value) {
                            matched = true;
                            executeIntern(body, jump, NULL);
                        }
                        if (code[pointer] == 0x6e) {
                            pointer++;
                            if (code[pointer] == 0x03) {
                                pointer++;
                                char elseCountLength[4];
                                for (int index = 0; index < 4; index++)
                                    elseCountLength[index] = code[pointer + index];
                                pointer += 4;
                                int elseCount = convertBytesToInt(elseCountLength);
                                for (int elseCounter = 0; elseCounter < elseCount; elseCounter++) {
                                    if (code[pointer] == 0x53) {
                                        pointer++;
                                        if (code[pointer] == 0x01) {
                                            pointer++;
                                            Dictionary* expDict2 = dict_new();
                                            expStack.push(&expStack, expDict2);
                                            calculate(1);
                                            BoolValue* elseCondition = (BoolValue*)dict_get((Dictionary*)expStack.pop(&expStack), "value");
                                            if (code[pointer] == 0x02) {
                                                pointer++;
                                                if (code[pointer] == 0x6f) {
                                                    pointer++;
                                                    char jumpBytes2[4];
                                                    for (int index = 0; index < (int)sizeof(jumpBytes2); index++)
                                                        jumpBytes2[index] = code[pointer + index];
                                                    pointer += (int)sizeof(jumpBytes2);
                                                    int jump2 = convertBytesToInt(jumpBytes2);
                                                    char body2[jump2];
                                                    for(unsigned long i = pointer; i < pointer + jump2; i++)
                                                        body2[i - pointer] = code[i];
                                                    pointer += jump2;
                                                    if (!matched && elseCondition->value) {
                                                        matched = true;
                                                        executeIntern(body2, jump2, NULL);
                                                    }
                                                    if (code[pointer] == 0x6e) {
                                                        pointer++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else if (code[pointer] == 0x54) {
                                        pointer++;
                                        if (code[pointer] == 0x01) {
                                            pointer++;
                                            if (code[pointer] == 0x6f) {
                                                pointer++;
                                                char jumpBytes2[4];
                                                for (int index = 0; index < (int)sizeof(jumpBytes2); index++)
                                                    jumpBytes2[index] = code[pointer + index];
                                                pointer += (int)sizeof(jumpBytes2);
                                                int jump2 = convertBytesToInt(jumpBytes2);
                                                char body2[jump2];
                                                for(unsigned long i = pointer; i < pointer + jump2; i++)
                                                    body2[i - pointer] = code[i];
                                                pointer += jump2;
                                                if (!matched)
                                                    executeIntern(body2, jump2, NULL);
                                                if (code[pointer] == 0x6e) {
                                                    pointer++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (code[pointer] == 0x53) {
            pointer++;
            unsigned long bodyStartPos = pointer;
            void* counter = NULL;
            while (true) {
                pointer = bodyStartPos;
                if (code[pointer] == 0x01) {
                    pointer++;
                    Dictionary* expDict = dict_new();

                    expStack.push(&expStack, expDict);
                    int tempMachineState = machineState;
                    calculate(1);
                    machineState = tempMachineState;
                    void* limitRaw = dict_get((Dictionary*)expStack.pop(&expStack), "value");
                    if (code[pointer] == 0x02) {
                        pointer++;
                        Dictionary* expDict2 = dict_new();

                        expStack.push(&expStack, expDict2);
                        tempMachineState = machineState;
                        calculate(1);
                        machineState = tempMachineState;
                        void* stepRaw = dict_get((Dictionary*)expStack.pop(&expStack), "value");
                        if (code[pointer] == 0x03) {
                            pointer++;
                            if (code[pointer] == 0x6f) {
                                pointer++;
                                char jumpBytes[4];
                                for (int index = 0; index < (int)sizeof(jumpBytes); index++)
                                    jumpBytes[index] = code[pointer + index];
                                pointer += (int)sizeof(jumpBytes);
                                int jump = convertBytesToInt(jumpBytes);
                                char body[jump];
                                for(unsigned long i = pointer; i < pointer + jump; i++)
                                    body[i - pointer] = code[i];
                                pointer += jump;
                                if (counter == NULL) {
                                    if (strcmp(((Value *) limitRaw)->valueType, "short") == 0) {
                                        counter = ((ShortValue *) limitRaw);
                                        ((ShortValue *) counter)->value = 0;
                                    } else if (strcmp(((Value *) limitRaw)->valueType, "int") == 0) {
                                        counter = ((IntValue *) limitRaw);
                                        ((IntValue *) counter)->value = 0;
                                    } else if (strcmp(((Value *) limitRaw)->valueType, "long") == 0) {
                                        counter = ((LongValue *) limitRaw);
                                        ((LongValue *) counter)->value = 0;
                                    } else if (strcmp(((Value *) limitRaw)->valueType, "float") == 0) {
                                        counter = ((FloatValue *) limitRaw);
                                        ((FloatValue *) counter)->value = 0;
                                    } else if (strcmp(((Value *) limitRaw)->valueType, "double") == 0) {
                                        counter = ((DoubleValue *) limitRaw);
                                        ((DoubleValue *) counter)->value = 0;
                                    }
                                }
                                if (((BoolValue*)lt(counter, limitRaw))->value) {
                                    executeIntern(body, jump, NULL);
                                    counter = sum(counter, stepRaw);
                                }
                                else {
                                    if (code[pointer] == 0x6e) {
                                        pointer++;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (code[pointer] == 0x54) {
            pointer++;
            unsigned long bodyStartPos = pointer;
            while (true) {
                pointer = bodyStartPos;
                if (code[pointer] == 0x01) {
                    pointer++;
                    Dictionary *expDict = dict_new();

                    expStack.push(&expStack, expDict);
                    int tempMachineState = machineState;
                    calculate(1);
                    machineState = tempMachineState;
                    void *condition = dict_get((Dictionary *) expStack.pop(&expStack), "value");
                    if (code[pointer == 0x02]) {
                        pointer++;
                        if (code[pointer] == 0x6f) {
                            pointer++;
                            char jumpBytes[4];
                            for (int index = 0; index < (int)sizeof(jumpBytes); index++)
                                jumpBytes[index] = code[pointer + index];
                            pointer += (int)sizeof(jumpBytes);
                            int jump = convertBytesToInt(jumpBytes);
                            char body[jump];
                            for(unsigned long i = pointer; i < pointer + jump; i++)
                                body[i - pointer] = code[i];
                            pointer += jump;
                            if (((BoolValue *) condition)->value) {
                                executeIntern(body, jump, NULL);
                            } else {
                                if (code[pointer] == 0x6e) {
                                    pointer++;
                                }
                                break;
                            }
                            if (code[pointer] == 0x6e) {
                                pointer++;
                            }
                        }
                    }
                }
            }
        }
        else if (code[pointer] == 0x55) {
            pointer++;
            if (code[pointer] == 0x01) {
                pointer++;
                expStack.push(&expStack, dict_new());
                calculate(1);
                void* target = dict_get((Dictionary*)expStack.top(&expStack), "value");
                char* refStr = dict_get((Dictionary*)expStack.top(&expStack), "value2");
                Object* thisObj = dict_get((Dictionary*)expStack.pop(&expStack), "value3");
                if (code[pointer] == 0x02) {
                    pointer++;
                    char entriesCountBytes[4];
                    for (int index = 0; index < 4; index++)
                        entriesCountBytes[index] = code[pointer + index];
                    pointer += 4;
                    int entriesCount = convertBytesToInt(entriesCountBytes);
                    Dictionary* entriesDict = dict_new();
                    for (int counter = 0; counter < entriesCount; counter++) {
                        if (code[pointer] == 0x03) {
                            pointer++;
                            char keyLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                keyLengthBytes[index] = code[pointer + index];
                            pointer += 4;
                            int keyLength = convertBytesToInt(keyLengthBytes);
                            char keyBytes[keyLength];
                            for (int index = 0; index < keyLength; index++)
                                keyBytes[index] = code[pointer + index];
                            pointer += keyLength;
                            char *key = &keyBytes[0];
                            char valueLengthBytes[4];
                            for (int index = 0; index < 4; index++)
                                valueLengthBytes[index] = code[pointer + index];
                            pointer += 4;
                            int valueLength = convertBytesToInt(valueLengthBytes);
                            expStack.push(&expStack, dict_new());
                            calculate(1);
                            dict_add(entriesDict, key, dict_get((Dictionary *) expStack.pop(&expStack), "value"));
                        }
                    }
                    if (target == NULL) {
                        routeAndResolve(refStr, entriesDict);
                    } else {
                        Function* func = (Function*) target;
                        if (thisObj != NULL)
                            dict_add(entriesDict, "this", thisObj);
                        executeIntern(func->codes, func->loc, entriesDict);
                    }
                }
            }
        }
        else if (code[pointer] == 0x56) {
            pointer++;
            if (code[pointer] == 0x01) {
                pointer++;
                Dictionary* expDict = dict_new();
                expStack.push(&expStack, expDict);
                calculate(0);
                struct Reference* ref = (struct Reference*)dict_get((Dictionary*)expStack.pop(&expStack), "value");
                if (code[pointer] == 0x02) {
                    pointer++;
                    Dictionary* expDict2 = dict_new();
                    expStack.push(&expStack, expDict2);
                    calculate(1);
                    Object* objectChain = NULL;
                    void* exp = dict_get((Dictionary*)expStack.pop(&expStack), "value");
                    char* varNameStr = malloc(strlen(ref->currentChain->id) + 1);
                    strcpy(varNameStr, ref->currentChain->id);
                    struct StackDataItem* iterator = dataStack.item;
                    void* variable = dict_get(((Dictionary*)iterator->data), varNameStr);
                    if (variable != NULL && strcmp(((Code*)variable)->type, "Object") == 0)
                        objectChain = variable;
                    while (variable == NULL && iterator->prev != NULL) {
                        iterator = iterator->prev;
                        variable = dict_get((Dictionary*)iterator->data, varNameStr);
                    }
                    Identifier* finalChain = ref->currentChain;
                    ref = ref->restOfTheChain;
                    char* refStr = varNameStr;
                    while (variable != NULL && ref != NULL) {
                        if (strcmp(((Code*)variable)->type, "Object") == 0) {
                            void* variable2 = dict_get(((Object*)variable)->value, ref->currentChain->id);
                            if (variable == NULL) {
                                variable2 = dict_get(((Object *) variable)->funcs, ref->currentChain->id);
                                if (variable2 != NULL)
                                    variable = variable2;
                            }
                            else
                                variable = variable2;
                        }
                        else if (strcmp(((Code*)variable)->type, "Class") == 0) {
                            variable = dict_get(((Class*)variable)->functions, ref->currentChain->id);
                        }
                        strcat(refStr, ".");
                        strcat(refStr, ref->currentChain->id);
                        finalChain = ref->currentChain;
                        ref = ref->restOfTheChain;
                        if (variable != NULL && strcmp(((Code*)variable)->type, "Object") == 0)
                            objectChain = variable;
                    }
                    if (objectChain != NULL) {
                        char *varNameStr2 = malloc(strlen(finalChain->id) + 1);
                        strcpy(varNameStr2, finalChain->id);
                        dict_delete(objectChain->value, varNameStr2);
                        dict_add(objectChain->value, varNameStr2, exp);
                    } else {
                        if (variable == NULL) {
                            char *varNameStr2 = malloc(strlen(finalChain->id) + 1);
                            strcpy(varNameStr2, finalChain->id);
                            dict_add(((Dictionary *) dataStack.top(&dataStack)), varNameStr2, exp);
                        } else {
                            char *varNameStr2 = malloc(strlen(finalChain->id) + 1);
                            strcpy(varNameStr2, finalChain->id);
                            dict_delete((Dictionary *) iterator->data, varNameStr2);
                            dict_add((Dictionary *) iterator->data, varNameStr2, exp);
                        }
                    }
                }
            }
        }
        else if (code[pointer] == 0x59) {
            pointer++;
            if (code[pointer] == 0x01) {
                pointer++;
                expStack.push(&expStack, dict_new());
                calculate(1);
                return dict_get((Dictionary*)expStack.pop(&expStack), "value");
            }
        }
    }
}

void* executeIntern(char c[], unsigned long length, Dictionary* entriesDict) {
    if (entriesDict == NULL)
        dataStack.push(&dataStack, dict_new());
    else
        dataStack.push(&dataStack, entriesDict);
    bufferStack.push(&bufferStack, dict_new());
    CodePack* cp = malloc(sizeof(CodePack));
    cp->code = code;
    cp->loc = codeLength;
    cp->pointer = pointer;
    codeLengthStack.push(&codeLengthStack, cp);
    code = &c[0];
    codeLength = length;
    pointer = 0;
    void* returnValue = ride();
    cp = codeLengthStack.pop(&codeLengthStack);
    code = cp->code;
    codeLength = cp->loc;
    pointer = cp->pointer;
    bufferStack.pop(&bufferStack);
    dataStack.pop(&dataStack);
    return returnValue;
}

void execute(char* c, long length) {
    initStack(&codeLengthStack);
    initStack(&bufferStack);
    initStack(&expStack);
    initStack(&dataStack);
    executeIntern(c, length, NULL);
}