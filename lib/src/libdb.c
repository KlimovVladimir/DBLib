#include <libdb.h>

void hello()
{
    printf("hello, world\n");
}

int StoreDump(char *pathOutFile, StatData *data, int size)
{
    FILE *fileOut;

    fileOut = fopen(pathOutFile, "w");
    
    if (fileOut == NULL)
    {
        fprintf(stderr, "%s:Failed to open %s\n", __FUNCTION__, pathOutFile);
        return ERROR_OPEN_FILE;
    }

    for (int i = 0; i < size; i++)
        fprintf(fileOut, "%ld %d %f %u %u\n", data[i].id, data[i].count, data[i].cost, data[i].primary, data[i].mode);

    fclose(fileOut); 

    return SUCCESS;
}

int LoadDump(char *pathOutFile, StatData *data)
{
    FILE *fileOut;
    int count = 0;
    unsigned primary, mode;

    fileOut = fopen(pathOutFile, "r");
    
    if (fileOut == NULL)
    {
        fprintf(stderr, "%s:Failed to open %s\n", __FUNCTION__, pathOutFile);
        return ERROR_OPEN_FILE;
    }

    while (fscanf(fileOut, "%ld %d %f %u %u", &data[count].id, &data[count].count, &data[count].cost, &primary, &mode) != EOF)
    {
        if (primary > 1 || mode > 7)
        {
            fprintf(stderr, "%s:Readed invalid value at %d line from file %s\n", __FUNCTION__, count + 1, pathOutFile);
            fclose(fileOut); 
            return ERROR_INVALID_VALUE;
        }

        data[count].primary = primary;
        data[count].mode = mode;
        printf("| %ld | %d | %f | %u | %u |\n", data[count].id, data[count].count, data[count].cost, data[count].primary, data[count].mode);
        count++;
    }

    fclose(fileOut); 

    return count;
}

static inline int compareId(const void *a, const void *b)
{
  const StatData da = *((const StatData *) a);
  const StatData db = *((const StatData *) b);
  return (da.id < db.id) ? -1 : (da.id == db.id) ? 0 : 1;
}

int JoinDump(StatData *dataA, int sizeA, StatData *dataB, int sizeB, StatData *dataOut, int sizeOut)
{

    StatData OUTMASS[100] = { 0 };
    int OUTCOUNT = 0;

    int err;

    err = timsort(dataA, sizeA, sizeof(dataA[0]), compareId);
    if (err) {
        fprintf(stderr, "%s:Timsort returned error %d\n", __FUNCTION__, err);
    }
    else {
        for (int i = 0; i < sizeA; ++i) {
            printf("| %ld | %d | %f | %u | %u |\n", dataA[i].id, dataA[i].count, dataA[i].cost, dataA[i].primary, dataA[i].mode);
        }
        printf("|----------------------|\n");
    }

    err = timsort(dataB, sizeB, sizeof(dataB[0]), compareId);
    if (err) {
        fprintf(stderr, "%s:Timsort returned error %d\n", __FUNCTION__, err);
    }
    else {
        for (int i = 0; i < sizeB; ++i) {
            printf("| %ld | %d | %f | %u | %u |\n", dataB[i].id, dataB[i].count, dataB[i].cost, dataB[i].primary, dataB[i].mode);
        }
        printf("|----------------------|\n");
    }


    int maxSize = (sizeA > sizeB) ? sizeA : sizeB;
    for (int i,j = 0; i < sizeA || j < sizeB;)
    {
        if (i < sizeA && j < sizeB)
        {
            if (dataA[i].id == dataB[j].id)
            {
                OUTMASS[OUTCOUNT].id = dataA[i].id;
                OUTMASS[OUTCOUNT].count = OUTMASS[OUTCOUNT].count + dataA[i].count + dataB[j].count;
                OUTMASS[OUTCOUNT].cost = OUTMASS[OUTCOUNT].cost + dataA[i].cost + dataB[j].cost;
                OUTMASS[OUTCOUNT].primary = (OUTMASS[OUTCOUNT].primary == 0) ? OUTMASS[OUTCOUNT].primary : (dataA[i].primary == 0) ? dataA[i].primary : dataB[j].primary;
                OUTMASS[OUTCOUNT].mode = MAX(MAX(OUTMASS[OUTCOUNT].mode, dataA[i].mode), dataB[j].mode);

                i++;
                j++;
                continue;
            }
            else if (dataA[i].id < dataB[j].id)
            {
                OUTMASS[OUTCOUNT].id = dataA[i].id;
                OUTMASS[OUTCOUNT].count = dataA[i].count;
                OUTMASS[OUTCOUNT].cost = dataA[i].cost;
                OUTMASS[OUTCOUNT].primary = dataA[i].primary;
                OUTMASS[OUTCOUNT].mode = dataA[i].mode;
                OUTCOUNT++;
                i++;
                continue;
            }
            else
            {
                OUTMASS[OUTCOUNT].id = dataB[j].id;
                OUTMASS[OUTCOUNT].count = dataB[j].count;
                OUTMASS[OUTCOUNT].cost = dataB[j].cost;
                OUTMASS[OUTCOUNT].primary = dataB[j].primary;
                OUTMASS[OUTCOUNT].mode = dataB[j].mode;
                OUTCOUNT++;
                j++;
                continue;
            }
        }
        else if (i < sizeA)
        {
            OUTMASS[OUTCOUNT].id = dataA[i].id;
            OUTMASS[OUTCOUNT].count = dataA[i].count;
            OUTMASS[OUTCOUNT].cost = dataA[i].cost;
            OUTMASS[OUTCOUNT].primary = dataA[i].primary;
            OUTMASS[OUTCOUNT].mode = dataA[i].mode;
            OUTCOUNT++;
            i++;
        }
        else if (j < sizeB)
        {
            OUTMASS[OUTCOUNT].id = dataB[j].id;
            OUTMASS[OUTCOUNT].count = dataB[j].count;
            OUTMASS[OUTCOUNT].cost = dataB[j].cost;
            OUTMASS[OUTCOUNT].primary = dataB[j].primary;
            OUTMASS[OUTCOUNT].mode = dataB[j].mode;
            OUTCOUNT++;
            j++;
        }
    }

    for (int i = 0; i < OUTCOUNT; ++i) {
        printf("| %ld | %d | %f | %u | %u |\n", OUTMASS[i].id, OUTMASS[i].count, OUTMASS[i].cost, OUTMASS[i].primary, OUTMASS[i].mode);
    }

    return SUCCESS;
}

static inline int compareCost(const void *a, const void *b)
{
  const StatData da = *((const StatData *) a);
  const StatData db = *((const StatData *) b);
  return (da.cost < db.cost) ? -1 : (da.cost == db.cost) ? 0 : 1;
}

int SortDump(StatData *data, int size)
{
    int err;

    err = timsort(data, size, sizeof(data[0]), compareCost);
    if (err) {
        fprintf(stderr, "%s:Timsort returned error %d\n", __FUNCTION__, err);
    } else {
        for (int i = 0; i < size; ++i) {
            printf("| %ld | %d | %f | %u | %u |\n", data[i].id, data[i].count, data[i].cost, data[i].primary, data[i].mode);
        }
    }
    return SUCCESS;
}