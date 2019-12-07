#include <bits/mathcalls.h>
#include "Buffer.c"
#include "Models.h"
#include "Stack.h"
#include "Dictionary.h"
#include <math.h>
#include <float.h>

char* code;
int machineState = 0x00;
int pointer = 0;

Stack bufferStack;
Stack expStack;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

Value sum(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "int") == 0 ||
            strcmp(value1.type, "short") == 0 ||
            strcmp(value1.type, "long") == 0 ||
            strcmp(value1.type, "float") == 0 ||
            strcmp(value1.type, "double") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = (*((double *)value1.value) + *((double *)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
        else if (strcmp(value2.type, "string") == 0) {
            char str[(int)(ceil(log10((*((double*)value1.value)))+1)*sizeof(char))];
            sprintf(str, "%f", (*((double*)value1.value)));
            char* result = str + *((char*)value2.value);
            resValue.value = &result;
            resValue.type = "double";
            return resValue;
        } else if (strcmp(value2.type, "bool") == 0) {
            double result = *((double*)value1.value) + ((*(bool*)value2.value) ? 1 : 0);
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
    } else if (strcmp(value1.value, "string") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            char str[(int)(ceil(log10((*((double*)value2.value)))+1)*sizeof(char))];
            sprintf(str, "%f", (*((double*)value2.value)));
            char* result = str + *((char*)value1.value);
            resValue.value = &result;
            resValue.type = "double";
            return resValue;
        } else if (strcmp(value2.value, "string") == 0) {
            char* result = concat((char *) (value1.value), (char *) value2.value);
            resValue.value = &result;
            resValue.type = "string";
            return resValue;
        } else if (strcmp(value2.value, "bool") == 0) {
            char* boolStr = *((bool*)value2.value) ? "true" : "false";
            char* result = concat((char *) (value1.value), boolStr);
            resValue.value = &result;
            resValue.type = "string";
            return resValue;
        }
    } else if (strcmp(value1.value, "bool") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = ((*(bool*)value1.value) ? 1 : 0) + *((double*)value2.value);
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        } else if (strcmp(value2.value, "string") == 0) {
            char* result = concat((*(bool *) (value1.value) ? "true" : "false"), (char *) value2.value);
            resValue.value = &result;
            resValue.type = "string";
            return resValue;
        } else if (strcmp(value2.value, "bool") == 0) {
            bool result = *(bool*)(value1.value) || *(bool*)(value2.value);
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value subtract(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "int") == 0 ||
        strcmp(value1.type, "short") == 0 ||
        strcmp(value1.type, "long") == 0 ||
        strcmp(value1.type, "float") == 0 ||
        strcmp(value1.type, "double") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = (*((double*)value1.value) - *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
    }
}

Value multiply(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "int") == 0 ||
        strcmp(value1.type, "short") == 0 ||
        strcmp(value1.type, "long") == 0 ||
        strcmp(value1.type, "float") == 0 ||
        strcmp(value1.type, "double") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = (*((double*)value1.value) * *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
    }
}

Value divide(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "int") == 0 ||
        strcmp(value1.type, "short") == 0 ||
        strcmp(value1.type, "long") == 0 ||
        strcmp(value1.type, "float") == 0 ||
        strcmp(value1.type, "double") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = (*((double*)value1.value) / *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
    }
}

Value mod(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "int") == 0 ||
        strcmp(value1.type, "short") == 0 ||
        strcmp(value1.type, "long") == 0 ||
        strcmp(value1.type, "float") == 0 ||
        strcmp(value1.type, "double") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = *(int *) value1.value % *(int *) value2.value;
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
    }
}

Value power(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "int") == 0 ||
        strcmp(value1.type, "short") == 0 ||
        strcmp(value1.type, "long") == 0 ||
        strcmp(value1.type, "float") == 0 ||
        strcmp(value1.type, "double") == 0) {
        if (strcmp(value2.type, "int") == 0 ||
            strcmp(value2.type, "short") == 0 ||
            strcmp(value2.type, "long") == 0 ||
            strcmp(value2.type, "float") == 0 ||
            strcmp(value2.type, "double") == 0) {
            double result = pow(*((double*)value1.value), *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.type = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.type = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.type = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.type = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.type = "double";
                    return resValue;
                }
            }
        }
    }
}

Value and(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) && *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value or(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) || *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value equal(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) == *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value ne(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) != *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value lt(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) < *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value le(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) <= *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value ge(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) >= *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Value gt(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.type, "bool") == 0) {
        if (strcmp(value2.type, "bool") == 0) {
            bool result = (*((bool*)value1.value) > *((bool*)value2.value));
            resValue.value = &result;
            resValue.type = "bool";
            return resValue;
        }
    }
}

Exp calculate() {
    while (!expStack.isEmpty()) {
        if (machineState == 0x71) {
            if (code[pointer] == 0x01) {
                machineState = 0x711;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x72) {
            if (code[pointer] == 0x01) {
                machineState = 0x721;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x73) {
            if (code[pointer] == 0x01) {
                machineState = 0x731;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x74) {
            if (code[pointer] == 0x01) {
                machineState = 0x741;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x75) {
            if (code[pointer] == 0x01) {
                machineState = 0x751;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x76) {
            if (code[pointer] == 0x01) {
                machineState = 0x761;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x77) {
            if (code[pointer] == 0x01) {
                machineState = 0x771;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x78) {
            if (code[pointer] == 0x01) {
                machineState = 0x781;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x79) {
            if (code[pointer] == 0x01) {
                machineState = 0x791;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x7a) {
            if (code[pointer] == 0x01) {
                machineState = 0x7a1;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x7b) {
            if (code[pointer] == 0x01) {
                machineState = 0x7b1;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x7c) {
            if (code[pointer] == 0x01) {
                machineState = 0x7c1;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x7d) {
            if (code[pointer] == 0x01) {
                machineState = 0x7d1;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x7e) {
            if (code[pointer] == 0x01) {
                machineState = 0x7e1;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x55) {
            if (code[pointer] == 0x01) {
                machineState = 0x551;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x61) {
            if (code[pointer] == 0x01) {
                machineState = 0x611;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x62) {
            if (code[pointer] == 0x01) {
                machineState = 0x621;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x63) {
            if (code[pointer] == 0x01) {
                machineState = 0x631;
                expStack.push(&createDictionary());
                pointer++;
                continue;
            }
        } else if (machineState == 0x711) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x712;
            continue;
        } else if (machineState == 0x721) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x722;
            continue;
        } else if (machineState == 0x731) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x732;
            continue;
        } else if (machineState == 0x741) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x742;
            continue;
        } else if (machineState == 0x751) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x752;
            continue;
        } else if (machineState == 0x761) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x762;
            continue;
        } else if (machineState == 0x771) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x772;
            continue;
        } else if (machineState == 0x781) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x782;
            continue;
        } else if (machineState == 0x791) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x792;
            continue;
        } else if (machineState == 0x7a1) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x7a2;
            continue;
        } else if (machineState == 0x7b1) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x7b2;
            continue;
        } else if (machineState == 0x7c1) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x7c2;
            continue;
        } else if (machineState == 0x7d1) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x7d2;
            continue;
        } else if (machineState == 0x7e1) {
            pointer++;
            Exp value1 = calculate();
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x7e2;
            continue;
        } else if (machineState == 0x712) {
            pointer++;
            Exp value2 = calculate();
            Sum sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x722) {
            pointer++;
            Exp value2 = calculate();
            Subtract sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x732) {
            pointer++;
            Exp value2 = calculate();
            Multiply sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x742) {
            pointer++;
            Exp value2 = calculate();
            Division sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x752) {
            pointer++;
            Exp value2 = calculate();
            Mod sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x762) {
            pointer++;
            Exp value2 = calculate();
            Pow sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x772) {
            pointer++;
            Exp value2 = calculate();
            And sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x782) {
            pointer++;
            Exp value2 = calculate();
            Or sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x792) {
            pointer++;
            Exp value2 = calculate();
            Equal sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x7a2) {
            pointer++;
            Exp value2 = calculate();
            GT sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x7b2) {
            pointer++;
            Exp value2 = calculate();
            GE sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x7c2) {
            pointer++;
            Exp value2 = calculate();
            NE sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x7d2) {
            pointer++;
            Exp value2 = calculate();
            LE sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        } else if (machineState == 0x7e2) {
            pointer++;
            Exp value2 = calculate();
            LT sum;
            sum.value1 = *(Exp*)(((Dictionary*)expStack.top())->get("value1"));
            sum.value2 = value2;
            expStack.pop();
            return *(Exp*)&sum;
        }

        if (code[pointer] == 0x71) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x72) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x73) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x74) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x75) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x76) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x77) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x78) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x79) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x7a) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x7b) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x7c) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x7d) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x7e) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x55) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x61) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x62) {
            machineState = code[pointer];
            pointer++;
        } else if (code[pointer] == 0x63) {
            machineState = code[pointer];
            pointer++;
        }
    }
}

void ride() {
    while (sizeof(code) > pointer) {
        if (machineState == 0x00) {
            if (code[pointer] == 0x51) {
                machineState = code[pointer];
                pointer++;
            } else if (code[pointer] == 0x52) {
                machineState = code[pointer];
                pointer++;
            } else if (code[pointer] == 0x53) {
                machineState = code[pointer];
                pointer++;
            } else if (code[pointer] == 0x54) {
                machineState = code[pointer];
                pointer++;
            } else if (code[pointer] == 0x55) {
                machineState = code[pointer];
                pointer++;
            } else if (code[pointer] == 0x56) {
                machineState = code[pointer];
                pointer++;
            }
        }
        else if (machineState == 0x51) {
            if (code[pointer] == 0x01) {
                machineState = 0x511;
                pointer++;
            }
        }
        else if (machineState == 0x52) {
            if (code[pointer] == 0x01) {
                machineState = 0x521;
                pointer++;
            }
        }
        else if (machineState == 0x53) {
            if (code[pointer] == 0x01) {
                machineState = 0x531;
                pointer++;
            }
        }
        else if (machineState == 0x54) {
            if (code[pointer] == 0x01) {
                machineState = 0x541;
                pointer++;
            }
        }
        else if (machineState == 0x55) {
            if (code[pointer] == 0x01) {
                machineState = 0x551;
                pointer++;
            }
        }
        else if (machineState == 0x56) {
            if (code[pointer] == 0x01) {
                machineState = 0x561;
                pointer++;
            }
        }
        else if (machineState == 0x511) {
            char funcNameLengthBytes[4];
            for (int index = 0; index < (int)sizeof(funcNameLengthBytes); index++)
                funcNameLengthBytes[index] = code[pointer + index];
            pointer += (int)sizeof(funcNameLengthBytes);
            int funcNameLength = *(int*)funcNameLengthBytes;
            char funcName[funcNameLength];
            for (int index = 0; index < funcNameLength; index++)
                funcName[index] = code[pointer + index];
            pointer += funcNameLength;
            bufferTableInsert(0x511, funcName);
            machineState = 0x512;
        } else if (machineState == 0x521) {
            char funcNameLengthBytes[4];
            for (int index = 0; index < (int)sizeof(funcNameLengthBytes); index++)
                funcNameLengthBytes[index] = code[pointer + index];
            pointer += (int)sizeof(funcNameLengthBytes);
            int funcNameLength = *(int*)funcNameLengthBytes;
            char funcName[funcNameLength];
            for (int index = 0; index < funcNameLength; index++)
                funcName[index] = code[pointer + index];
            pointer += funcNameLength;
            bufferTableInsert(0x521, funcName);
            machineState = 0x522;
        }
    }
}

void execute(char c[]) {
    strcpy(code, c);
    bufferStack = createStack();
    expStack = createStack();
    bufferSetup();
    ride();
}