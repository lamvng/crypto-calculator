char *readFile(char *fileName);
char* readFileByLine(char *fileName, int line);
char *readFileWithLength(char *fileName, int *size);
int writeFileByLine(char *fileName, char* data);
int writeNewFile(char *fileName, char* data);
int writeNewFileByLength(char *fileName, char *data, int size);