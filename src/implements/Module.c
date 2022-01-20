#include "../libs/Module.h"
#include <stdlib.h>
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
        buffer = (char *)malloc(stell + 1);
        if (buffer != NULL)
        {
            fread(buffer, sizeof(char), stell, fp);
            buffer[stell] = 0;
            fclose(fp);
            fp = NULL;
        }
    }
    return buffer;
}

char *readFileByLine(char *fileName, int line)
{
    int bufferLength = 1000;
    char *buffer = (char *)malloc((bufferLength+1) * sizeof(char));
    FILE *fptr;
    fptr = fopen(fileName, "r");
    if (fptr == NULL)
    {
        printf("No file %s\n", fileName);
        return NULL;
    }
    int j = 0;

    while (fgets(buffer, bufferLength, fptr))
    {
        // buffer;
        buffer[bufferLength] = '\0';
        if (j == line)
        {
            break;
        }
        j++;
    }
    fclose(fptr);

    return buffer;
}

char *readFileWithLength(char *fileName, int *size)
{
    char *buffer;
    FILE *fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        fseek(fp, 0L, SEEK_END);
        *size = (int) ftell(fp);
        rewind(fp);
        buffer = (char *)malloc(*size + 1);
        if (buffer != NULL)
        {
            fread(buffer, sizeof(char), *size, fp);
            buffer[*size] = 0;
            fclose(fp);
            fp = NULL;
        }
    }
    return buffer;
}

int writeFileByLine(char *fileName, char *data)
{
    FILE *fp;
    fp = fopen(fileName, "a");
    fprintf(fp, "%s", data);
    fclose(fp);
    return 0;
}

int writeNewFile(char *fileName, char *data)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    fprintf(fp, "%s", data);
    fclose(fp);
    return 0;
}

int writeNewFileByLength(char *fileName, char *data, int size)
{
    FILE *fp;
    fp = fopen(fileName, "wb");
    fwrite(data, sizeof(char), size, fp);
    fclose(fp);
    return 0;
}