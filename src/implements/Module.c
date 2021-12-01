#include "../libs/Module.h"

#include <stdio.h>

char *readFile(char *fileName)
{
    char *buffer;
    FILE *fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        fseek(fp, 0L, SEEK_END);
        long stell = ftell(fp);
        rewind(fp);
        buffer = (char *)malloc(stell +1);
        if (buffer != NULL)
        {
            fread(buffer, stell, 1, fp);
            fclose(fp);
            fp = NULL;
        }
    }
    return buffer;
}

int writeFileByLine(char *fileName, char* data){
    FILE *fp;
    fp = fopen(fileName, "a");
    fprintf(fp, "%s", data);
    fclose(fp);
    return 0;
}