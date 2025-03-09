#include <libdb.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"

char *uintToBinary(unsigned int num)
{
    char *res = malloc(4);
    int j = 0;
    
    for (int i = 2; i >= 0; i--, j++) {
        unsigned int bit = (num >> i) & 1;
        res[j] = bit + '0';
    }

    res[j] = '\0';
    return res;
}

int main(int argc, char *argv[])
{
    int error;

    if (argc != 4) {
        fprintf(stderr, RED "%s:Wrong count of arguments\n" RESET, __FUNCTION__);
        return 1;
    }

    StatData *dataA;
    int sizeA = 0;
    error = LoadDump(argv[1], &dataA, &sizeA);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:LoadDump dataA failed. Error:%d\n" RESET, __FUNCTION__, error);
        return 1;
    }

    StatData *dataB;
    int sizeB = 0;
    error = LoadDump(argv[2], &dataB, &sizeB);

    if (error != SUCCESS) {
        free(dataA);
        fprintf(stderr, RED "%s:LoadDump dataB failed. Error:%d\n" RESET, __FUNCTION__, error);
        return 1;
    }

    StatData *dataOut;
    int sizeOut = 0;
    error = JoinDump(dataA, sizeA, dataB, sizeB, &dataOut, &sizeOut);

    free(dataA);
    free(dataB);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:JoinDump failed. Error:%d\n" RESET, __FUNCTION__, error);
        return 1;
    }

    error = SortDump(dataOut, sizeOut);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:SortDump failed. Error:%d\n" RESET, __FUNCTION__, error);
        free(dataOut);
        return 1;
    }

    int size = sizeOut >= 10 ? 10 : sizeOut;
    printf("|------------DButil: final array----------|\n", __FUNCTION__);
    for (int i = 0; i < size; ++i) {
        char *res = uintToBinary(dataOut[i].mode);
        printf("|%d:| 0x%lx | %d | %.3E | %c | %s |\n", i, dataOut[i].id, dataOut[i].count, dataOut[i].cost, (dataOut[i].primary) ? 'y' : 'n', uintToBinary(dataOut[i].mode));
        free(res);
    }
    printf("|---------------------------------|\n\n");

    error = StoreDump(argv[3], dataOut, sizeOut);
    free(dataOut);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:SortDump failed. Error:%d\n" RESET, __FUNCTION__, error);
        return 1;
    }

    return 0;
}