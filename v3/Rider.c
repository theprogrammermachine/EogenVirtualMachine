#include "Rider.h"

char* code;
int machineState = 0x00;
int pointer = 0;

Stack bufferStack;
Stack expStack;
Stack dataStack;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

Value sum(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "int") == 0 ||
        strcmp(value1.valueType, "short") == 0 ||
        strcmp(value1.valueType, "long") == 0 ||
        strcmp(value1.valueType, "float") == 0 ||
        strcmp(value1.valueType, "double") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = (*((double *)value1.value) + *((double *)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
        else if (strcmp(value2.valueType, "string") == 0) {
            char str[(int)(ceil(log10((*((double*)value1.value)))+1)*sizeof(char))];
            sprintf(str, "%f", (*((double*)value1.value)));
            char* result = str + *((char*)value2.value);
            resValue.value = &result;
            resValue.valueType = "double";
            return resValue;
        } else if (strcmp(value2.valueType, "bool") == 0) {
            double result = *((double*)value1.value) + ((*(bool*)value2.value) ? 1 : 0);
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
    } else if (strcmp(value1.value, "string") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            char str[(int)(ceil(log10((*((double*)value2.value)))+1)*sizeof(char))];
            sprintf(str, "%f", (*((double*)value2.value)));
            char* result = str + *((char*)value1.value);
            resValue.value = &result;
            resValue.valueType = "double";
            return resValue;
        } else if (strcmp(value2.value, "string") == 0) {
            char* result = concat((char *) (value1.value), (char *) value2.value);
            resValue.value = &result;
            resValue.valueType = "string";
            return resValue;
        } else if (strcmp(value2.value, "bool") == 0) {
            char* boolStr = *((bool*)value2.value) ? "true" : "false";
            char* result = concat((char *) (value1.value), boolStr);
            resValue.value = &result;
            resValue.valueType = "string";
            return resValue;
        }
    } else if (strcmp(value1.value, "bool") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = ((*(bool*)value1.value) ? 1 : 0) + *((double*)value2.value);
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        } else if (strcmp(value2.value, "string") == 0) {
            char* result = concat((*(bool *) (value1.value) ? "true" : "false"), (char *) value2.value);
            resValue.value = &result;
            resValue.valueType = "string";
            return resValue;
        } else if (strcmp(value2.value, "bool") == 0) {
            bool result = *(bool*)(value1.value) || *(bool*)(value2.value);
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value subtract(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "int") == 0 ||
        strcmp(value1.valueType, "short") == 0 ||
        strcmp(value1.valueType, "long") == 0 ||
        strcmp(value1.valueType, "float") == 0 ||
        strcmp(value1.valueType, "double") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = (*((double*)value1.value) - *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

Value multiply(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "int") == 0 ||
        strcmp(value1.valueType, "short") == 0 ||
        strcmp(value1.valueType, "long") == 0 ||
        strcmp(value1.valueType, "float") == 0 ||
        strcmp(value1.valueType, "double") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = (*((double*)value1.value) * *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

Value divide(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "int") == 0 ||
        strcmp(value1.valueType, "short") == 0 ||
        strcmp(value1.valueType, "long") == 0 ||
        strcmp(value1.valueType, "float") == 0 ||
        strcmp(value1.valueType, "double") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = (*((double*)value1.value) / *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

Value mod(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "int") == 0 ||
        strcmp(value1.valueType, "short") == 0 ||
        strcmp(value1.valueType, "long") == 0 ||
        strcmp(value1.valueType, "float") == 0 ||
        strcmp(value1.valueType, "double") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = *(int *) value1.value % *(int *) value2.value;
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

Value power(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "int") == 0 ||
        strcmp(value1.valueType, "short") == 0 ||
        strcmp(value1.valueType, "long") == 0 ||
        strcmp(value1.valueType, "float") == 0 ||
        strcmp(value1.valueType, "double") == 0) {
        if (strcmp(value2.valueType, "int") == 0 ||
            strcmp(value2.valueType, "short") == 0 ||
            strcmp(value2.valueType, "long") == 0 ||
            strcmp(value2.valueType, "float") == 0 ||
            strcmp(value2.valueType, "double") == 0) {
            double result = pow(*((double*)value1.value), *((double*)value2.value));
            if (floor(result) == result) {
                if (result < INT16_MAX) {
                    short r = (short)result;
                    resValue.value = &r;
                    resValue.valueType = "short";
                    return resValue;
                }
                else if (result < INT32_MAX) {
                    int r = (int)result;
                    resValue.value = &r;
                    resValue.valueType = "int";
                    return resValue;
                }
                else if (result < INT64_MAX) {
                    long r = (long)result;
                    resValue.value = &r;
                    resValue.valueType = "long";
                    return resValue;
                }
            }
            else {
                if (result < FLT_MAX) {
                    float r = (float)result;
                    resValue.value = &r;
                    resValue.valueType = "float";
                    return resValue;
                }
                else if (result < DBL_MAX) {
                    double r = (double)result;
                    resValue.value = &r;
                    resValue.valueType = "double";
                    return resValue;
                }
            }
        }
    }
}

Value and(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) && *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value or(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) || *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value equal(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) == *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value ne(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) != *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value lt(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) < *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value le(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) <= *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value ge(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) >= *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Value gt(Value value1, Value value2) {
    Value resValue;
    if (strcmp(value1.valueType, "bool") == 0) {
        if (strcmp(value2.valueType, "bool") == 0) {
            bool result = (*((bool*)value1.value) > *((bool*)value2.value));
            resValue.value = &result;
            resValue.valueType = "bool";
            return resValue;
        }
    }
}

Exp oopCalculate(Exp exp) {

}

void oopRide(List codes) {
    while (codes.iteratorHasNext()) {
        Code* c = codes.iteratorForward();
        if (strcmp(c->type, "If") == 0) {
            Exp condition = oopCalculate(((If*) c)->condition);
            if ((bool) (*(Value *) &condition).value == true) {
                dataStack.push(&createDictionary);
                oopRide(((If*) c)->codes);
                dataStack.pop();
            }
        }
        else if (strcmp(c->type, "CounterLoop") == 0) {
            Identifier counter = ((CounterLoop*) c)->counter;
            Exp limit = oopCalculate(((CounterLoop*) c)->limit);
            Exp step = oopCalculate(((CounterLoop*) c)->step);
            for (int i = 0
                    ; i < *(int*)(*(Value*)&limit).value
                    ; i += *(int*)(*(Value*)&step).value) {
                Dictionary dataDict = createDictionary();
                Value counterValue;
                counterValue.valueType = "Integer";
                counterValue.value = &i;
                dataDict.put(counter.id, &counterValue);
                dataStack.push(&dataDict);
                oopRide(((CounterLoop*) c)->codes);
                dataStack.pop();
                step = oopCalculate(((CounterLoop*) c)->step);
            }
        }
        else if (strcmp(c->type, "ConditionalLoop") == 0) {
            Exp condition = oopCalculate(((ConditionalLoop*) c)->condition);
            while ((bool) (*(Value *) &condition).value == true) {
                dataStack.push(&createDictionary);
                oopRide(((ConditionalLoop*) c)->codes);
                dataStack.pop();
                condition = oopCalculate(((ConditionalLoop*) c)->condition);
            }
        }
    }
}

Exp calculate() {
    while (!expStack.isEmpty()) {
        if (machineState == 0x71) {
            if (code[pointer] == 0x01) {
                machineState = 0x711;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x72) {
            if (code[pointer] == 0x01) {
                machineState = 0x721;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x73) {
            if (code[pointer] == 0x01) {
                machineState = 0x731;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x74) {
            if (code[pointer] == 0x01) {
                machineState = 0x741;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x75) {
            if (code[pointer] == 0x01) {
                machineState = 0x751;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x76) {
            if (code[pointer] == 0x01) {
                machineState = 0x761;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x77) {
            if (code[pointer] == 0x01) {
                machineState = 0x771;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x78) {
            if (code[pointer] == 0x01) {
                machineState = 0x781;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x79) {
            if (code[pointer] == 0x01) {
                machineState = 0x791;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x7a) {
            if (code[pointer] == 0x01) {
                machineState = 0x7a1;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x7b) {
            if (code[pointer] == 0x01) {
                machineState = 0x7b1;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x7c) {
            if (code[pointer] == 0x01) {
                machineState = 0x7c1;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x7d) {
            if (code[pointer] == 0x01) {
                machineState = 0x7d1;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x7e) {
            if (code[pointer] == 0x01) {
                machineState = 0x7e1;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x55) {
            if (code[pointer] == 0x01) {
                machineState = 0x551;
                Dictionary dict = createDictionary();
                expStack.push(&dict);
                pointer++;
                continue;
            }
        } else if (machineState == 0x61) {
            pointer++;
            char idNameLengthArr[4];
            for (int index = 0; index < (int)sizeof(idNameLengthArr); index++)
                idNameLengthArr[index] = code[pointer + index];
            pointer += (int)sizeof(idNameLengthArr);
            int idNameLength = *(int*)idNameLengthArr;
            char idNameArr[idNameLength];
            for (int index = 0; index < (int)sizeof(idNameArr); index++)
                idNameArr[index] = code[pointer + index];
            pointer += (int)sizeof(idNameArr);
            char* idName = idNameArr;
            Identifier id;
            id.id = idName;
            id.exp.type = "Identifier";
            return *(Exp*)&id;
        } else if (machineState == 0x62) {
            pointer++;
            char valueLengthArr[4];
            for (int index = 0; index < (int)sizeof(valueLengthArr); index++)
                valueLengthArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueLengthArr);
            int valueLength = *(int*)valueLengthArr;
            char valueArr[valueLength];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* value = valueArr;
            Value val;
            val.value = &value;
            val.valueType = "string";
            val.exp.type = "Value";
            return *(Exp*)&val;
        } else if (machineState == 0x63) {
            pointer++;
            char valueArr[8];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            Value val;
            val.value = &value;
            val.valueType = "double";
            val.exp.type = "Value";
            return *(Exp*)&val;
        } else if (machineState == 0x64) {
            pointer++;
            char valueArr[4];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            Value val;
            val.value = &value;
            val.valueType = "float";
            val.exp.type = "Value";
            return *(Exp*)&val;
        } else if (machineState == 0x65) {
            pointer++;
            char valueArr[2];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            Value val;
            val.value = &value;
            val.valueType = "short";
            val.exp.type = "Value";
            return *(Exp*)&val;
        } else if (machineState == 0x66) {
            pointer++;
            char valueArr[4];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            Value val;
            val.value = &value;
            val.valueType = "integer";
            val.exp.type = "Value";
            return *(Exp*)&val;
        } else if (machineState == 0x67) {
            pointer++;
            char valueArr[8];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            Value val;
            val.value = &value;
            val.valueType = "long";
            val.exp.type = "Value";
            return *(Exp*)&val;
        } else if (machineState == 0x68) {
            pointer++;
            char valueArr[1];
            for (int index = 0; index < (int)sizeof(valueArr); index++)
                valueArr[index] = code[pointer + index];
            pointer += (int)sizeof(valueArr);
            char* valueRaw = valueArr;
            double value;
            memcpy(&value, valueRaw, sizeof(double));
            Value val;
            val.value = &value;
            val.valueType = "boolean";
            val.exp.type = "Value";
            return *(Exp*)&val;
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
            Exp value1Raw = calculate();
            Value value1 = *(Value*)&value1Raw;
            ((Dictionary*)expStack.top())->put("value1", &value1);
            if (code[pointer] == 0x02)
                machineState = 0x7e2;
            continue;
        } else if (machineState == 0x712) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = sum(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x722) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = subtract(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x732) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = multiply(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x742) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = divide(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x752) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = mod(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x762) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = power(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x772) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = and(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x782) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = or(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x792) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = equal(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x7a2) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = gt(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x7b2) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = ge(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x7c2) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = ne(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x7d2) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = le(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x7e2) {
            pointer++;
            Value value1 = *(Value*)(((Dictionary*)expStack.top())->get("value1"));
            Exp value2Raw = calculate();
            Value value2 = *(Value*)&value2Raw;
            expStack.pop();
            Value result = lt(value1, value2);
            return *(Exp*)&result;
        } else if (machineState == 0x55) {
            if (code[pointer] == 0x02) {
                char entriesCountArr[4];
                for (int index = 0; index < (int)sizeof(entriesCountArr); index++)
                    entriesCountArr[index] = code[pointer + index];
                pointer += (int)sizeof(entriesCountArr);
                int entriesCount = *(int*)entriesCountArr;
                Dictionary entriesDict = createDictionary();
                for (int counter = 0; counter < entriesCount; counter++) {
                    if (code[pointer] == 0x03) {
                        pointer++;
                        char keyLengthArr[4];
                        for (int index = 0; index < (int)sizeof(keyLengthArr); index++)
                            keyLengthArr[index] = code[pointer + index];
                        pointer += (int)sizeof(keyLengthArr);
                        int keyLength = *(int*)keyLengthArr;
                        char keyArr[keyLength];
                        for (int index = 0; index < (int)sizeof(keyArr); index++)
                            keyArr[index] = code[pointer + index];
                        pointer += (int)sizeof(keyArr);
                        char* key = keyArr;
                        char valueLengthArr[4];
                        for (int index = 0; index < (int)sizeof(valueLengthArr); index++)
                            valueLengthArr[index] = code[pointer + index];
                        pointer += (int)sizeof(valueLengthArr);
                        Exp value = calculate();
                        entriesDict.put(key, &value);
                    }
                }
                Exp funcRef = *(Exp*)(((Dictionary*)bufferStack.top())->get("funcRef"));
                if (strcmp(funcRef.type, "Identifier") == 0) {
                    Identifier id = *(Identifier*)&funcRef;
                    Function func = *(Function*)((Dictionary*)dataStack.top())->get(id.id);

                }
            }
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
            Exp funcRef = calculate();
            ((Dictionary*)bufferStack.top())->put("funcRef", &funcRef);
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

List ride(bool execute) {
    List scope = createList();
    while (sizeof(code) > pointer) {
        if (machineState == 0x00) {
            if (code[pointer] == 0x51) {
                machineState = code[pointer];
                pointer++;
                if (code[pointer] == 0x01) {
                    machineState = 0x511;
                    pointer++;
                    char funcNameLengthBytes[4];
                    for (int index = 0; index < (int)sizeof(funcNameLengthBytes); index++)
                        funcNameLengthBytes[index] = code[pointer + index];
                    pointer += (int)sizeof(funcNameLengthBytes);
                    int funcNameLength = *(int*)funcNameLengthBytes;
                    char funcName[funcNameLength];
                    for (int index = 0; index < funcNameLength; index++)
                        funcName[index] = code[pointer + index];
                    pointer += funcNameLength;
                    ((Dictionary*)bufferStack.top())->put("funcName", funcName);
                    machineState = 0x512;
                    if (code[pointer] == 0x02) {
                        char funcLevelLengthBytes[4];
                        for (int index = 0; index < (int)sizeof(funcLevelLengthBytes); index++)
                            funcLevelLengthBytes[index] = code[pointer + index];
                        pointer += (int)sizeof(funcLevelLengthBytes);
                        int funcLevelLength = *(int*)funcLevelLengthBytes;
                        char funcLevelStr[funcLevelLength];
                        for (int index = 0; index < funcLevelLength; index++)
                            funcLevelStr[index] = code[pointer + index];
                        pointer += funcLevelLength;
                        ((Dictionary*)bufferStack.top())->put("funcName", funcLevelStr);
                        machineState = 0x513;
                        if (code[pointer] == 0x03) {
                            List identifiers = createList();
                            char paramsCountBytes[4];
                            for (int index = 0; index < 4; index++)
                                paramsCountBytes[index] = code[pointer + index];
                            pointer += 4;
                            int paramsCount = *(int*)paramsCountBytes;
                            for (int counter = 0; counter < paramsCount; counter++) {
                                char paramLengthBytes[4];
                                for (int index = 0; index < (int)sizeof(paramLengthBytes); index++)
                                    paramLengthBytes[index] = code[pointer + index];
                                pointer += (int)sizeof(paramLengthBytes);
                                int paramLength = *(int*)paramLengthBytes;
                                pointer += paramLength;
                                char idStr[paramLength];
                                for (int index = 0; index < paramLength; index++)
                                    idStr[index] = code[pointer + index];
                                pointer += paramLength;
                                char* id = idStr;
                                identifiers.append(id);
                            }
                            machineState = 0x514;
                            if (code[pointer] == 0x04) {
                                pointer++;
                                if (code[pointer] == 0x06f) {
                                    pointer++;
                                    Dictionary dict = createDictionary();
                                    bufferStack.push(&dict);
                                    List codes = ride(false);
                                    bufferStack.pop();
                                    if (code[pointer] == 0x6e) {
                                        pointer++;
                                        Function function;
                                        function.funcName = funcName;
                                        function.params = identifiers;
                                        function.codes = codes;
                                        if (execute)
                                            scope.append(&function);
                                        ((Dictionary*)dataStack.top())->put(funcName, &function);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (code[pointer] == 0x55) {
                machineState = code[pointer];
                pointer++;
                if (code[pointer] == 0x01) {
                    pointer++;
                    Exp funcRefRaw = calculate();
                    Identifier funcRef = * (Identifier*) &funcRefRaw;
                    if (code[pointer] == 0x02) {
                        pointer++;
                        char entriesCountBytes[4];
                        for (int index = 0; index < 4; index++)
                            entriesCountBytes[index] = code[pointer + index];
                        pointer += 4;
                        int entriesCount = *(int*)entriesCountBytes;
                        Dictionary entriesDict = createDictionary();
                        for (int counter = 0; counter < entriesCount; counter++) {
                            if (code[pointer] == 0x03) {
                                pointer++;
                                char keyLengthBytes[4];
                                for (int index = 0; index < 4; index++)
                                    keyLengthBytes[index] = code[pointer + index];
                                pointer += 4;
                                int keyLength = *(int*)keyLengthBytes;
                                char keyBytes[keyLength];
                                for (int index = 0; index < keyLength; index++)
                                    keyBytes[index] = code[pointer + index];
                                pointer += 4;
                                char* key = keyBytes;
                                char valueLengthBytes[4];
                                for (int index = 0; index < 4; index++)
                                    valueLengthBytes[index] = code[pointer + index];
                                pointer += 4;
                                int valueLength = *(int*)valueLengthBytes;
                                Exp value = calculate();
                                entriesDict.put(key, &value);
                            }
                            Call call;
                            call.funcRef = &funcRef;
                            call.entries = entriesDict;
                            if (execute)
                                scope.append(&call);
                            routeAndResolve(funcRef.id, entriesDict);
                        }
                    }
                }
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
            ((Dictionary*)bufferStack.top())->put("funcName", funcName);
            machineState = 0x522;
        }
    }
}

void execute(char c[]) {
    strcpy(code, c);
    bufferStack = createStack();
    expStack = createStack();
    dataStack = createStack();
    ride(false);
}