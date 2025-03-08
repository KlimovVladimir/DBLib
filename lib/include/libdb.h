#include <stdio.h>
#include "timsort.h"


#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define SUCCESS 0
#define ERROR_OPEN_FILE -1
#define ERROR_INVALID_VALUE -2

typedef struct StatData {
    long id;
    int count;
    float cost;
    unsigned int primary:1;
    unsigned int mode:3;
} StatData;

int StoreDump(char *pathOutFile, StatData *data, int size);
int LoadDump(char *pathOutFile, StatData *data);
int JoinDump(StatData *dataA, int sizeA, StatData *dataB, int sizeB, StatData *dataOut, int sizeOut);
int SortDump(StatData *data, int size);

void hello();