
#include "Rider.h"

int main() {
    FILE *filePtr;
    char *buffer;
    long fileLen;
    filePtr = fopen("output.tra", "rb");
    fseek(filePtr, 0, SEEK_END);
    fileLen = ftell(filePtr);
    rewind(filePtr);
    buffer = (char *)malloc((fileLen + 1) * sizeof(char));
    fread(buffer, fileLen, 1, filePtr);
    fclose(filePtr);
    int i;
    for (i = 0; i < fileLen + 1; i++)
    {
        if (i > 0) printf(":");
        printf("%02X", buffer[i]);
    }
    printf("\n");
    execute(buffer, fileLen);
    return 0;
}