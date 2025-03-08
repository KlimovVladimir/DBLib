#include <libdb.h>

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
    if (argc != 4) {
        fprintf(stderr, "%s:Wrong count of arguments\n", __FUNCTION__);
        return 1;
    }

    StatData *dataA;
    int sizeA = 0;
    LoadDump(argv[1], &dataA, &sizeA);

    StatData *dataB;
    int sizeB = 0;
    LoadDump(argv[2], &dataB, &sizeB);

    StatData *dataOut;
    int sizeOut = 0;
    JoinDump(dataA, sizeA, dataB, sizeB, &dataOut, &sizeOut);

    SortDump(dataOut, sizeOut);

    int size = sizeOut >= 10 ? 10 : sizeOut;
    printf("|------------DButil: sorted array by cost----------|\n", __FUNCTION__);
    for (int i = 0; i < size; ++i) {
        char *res = uintToBinary(dataOut[i].mode);
        printf("|%d:| 0x%lx | %d | %.3E | %c | %s |\n", i, dataOut[i].id, dataOut[i].count, dataOut[i].cost, (dataOut[i].primary) ? 'y' : 'n', uintToBinary(dataOut[i].mode));
        free(res);
    }
    printf("|---------------------------------|\n\n");

    StoreDump(argv[3], dataOut, sizeOut);

    return 0;
}