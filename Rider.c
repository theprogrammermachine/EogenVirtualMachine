#include "Buffer.c"

char* code;
int machineState = 0x00;

void calculate(int pointer) {

    if (machineState == 0x71) {
        if (code[pointer] == 0x01) {
            machineState = 0x711;
            pointer++;
        }
    } else if (machineState == 0x72) {
        if (code[pointer] == 0x01) {
            machineState = 0x721;
            pointer++;
        }
    } else if (machineState == 0x73) {
        if (code[pointer] == 0x01) {
            machineState = 0x731;
            pointer++;
        }
    } else if (machineState == 0x74) {
        if (code[pointer] == 0x01) {
            machineState = 0x741;
            pointer++;
        }
    } else if (machineState == 0x75) {
        if (code[pointer] == 0x01) {
            machineState = 0x751;
            pointer++;
        }
    } else if (machineState == 0x76) {
        if (code[pointer] == 0x01) {
            machineState = 0x761;
            pointer++;
        }
    } else if (machineState == 0x77) {
        if (code[pointer] == 0x01) {
            machineState = 0x771;
            pointer++;
        }
    } else if (machineState == 0x78) {
        if (code[pointer] == 0x01) {
            machineState = 0x781;
            pointer++;
        }
    } else if (machineState == 0x79) {
        if (code[pointer] == 0x01) {
            machineState = 0x791;
            pointer++;
        }
    } else if (machineState == 0x7a) {
        if (code[pointer] == 0x01) {
            machineState = 0x7a1;
            pointer++;
        }
    } else if (machineState == 0x7b) {
        if (code[pointer] == 0x01) {
            machineState = 0x7b1;
            pointer++;
        }
    } else if (machineState == 0x7c) {
        if (code[pointer] == 0x01) {
            machineState = 0x7c1;
            pointer++;
        }
    } else if (machineState == 0x7d) {
        if (code[pointer] == 0x01) {
            machineState = 0x7d1;
            pointer++;
        }
    } else if (machineState == 0x7e) {
        if (code[pointer] == 0x01) {
            machineState = 0x7e1;
            pointer++;
        }
    } else if (machineState == 0x55) {
        if (code[pointer] == 0x01) {
            machineState = 0x551;
            pointer++;
        }
    } else if (machineState == 0x61) {
        if (code[pointer] == 0x01) {
            machineState = 0x611;
            pointer++;
        }
    } else if (machineState == 0x62) {
        if (code[pointer] == 0x01) {
            machineState = 0x621;
            pointer++;
        }
    } else if (machineState == 0x63) {
        if (code[pointer] == 0x01) {
            machineState = 0x631;
            pointer++;
        }
    } else if (machineState == 0x711) {

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
    
    calculate(pointer);
}

void ride(int pointer) {
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
    bufferSetup();
    ride(0);
}