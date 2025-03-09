#include <libdb.h>

int StoreDump(char *pathOutFile, StatData *data, int size)
{
    FILE *fileOut;

    fileOut = fopen(pathOutFile, "w");
    
    if (fileOut == NULL) {
        fprintf(stderr, "%s:Failed to open %s\n", __FUNCTION__, pathOutFile);
        return ERROR_OPEN_FILE;
    }

    for (int i = 0; i < size; i++)
        fprintf(fileOut, "%ld %d %f %u %u\n", data[i].id, data[i].count, data[i].cost, data[i].primary, data[i].mode);

    fclose(fileOut);

    return SUCCESS;
}

int LoadDump(char *pathOutFile, StatData **data, int* size)
{
    FILE *fileOut;
    long id;
    int count;
    float cost;
    unsigned primary, mode;

    fileOut = fopen(pathOutFile, "r");
    
    if (fileOut == NULL) {
        fprintf(stderr, "%s:Failed to open %s\n", __FUNCTION__, pathOutFile);
        return ERROR_OPEN_FILE;
    }

    *data = malloc(sizeof(StatData));

#ifdef DUMP_ARRAYS
    printf("|------------%s %s----------|\n", __FUNCTION__, pathOutFile);
#endif

    while (fscanf(fileOut, "%ld %d %f %u %u", &id, &count, &cost, &primary, &mode) != EOF) {
        /* Validating of primary and mode fields */
        if (primary > 1 || mode > 7) {
            fprintf(stderr, "%s:Readed invalid value at %d line from file %s\n", __FUNCTION__, *size + 1, pathOutFile);
            fclose(fileOut);
            free(*data);
            return ERROR_INVALID_VALUE;
        }

        /* Reallocation memory for result array */
        if (*size > 0) {
            StatData *dataTmp = realloc(*data, (sizeof(StatData) * ((*size) + 1)));
            if (dataTmp == NULL) {
                free(*data);
                return ERROR_REALLOC_FAILURE;
            } else {
                *data = dataTmp;
            }
        }

        (*data)[*size].id = id;
        (*data)[*size].count = count;
        (*data)[*size].cost = cost;
        (*data)[*size].primary = primary;
        (*data)[*size].mode = mode;
#ifdef DUMP_ARRAYS
        printf("|%d:| %ld | %d | %f | %u | %u |\n", *size, (*data)[*size].id, (*data)[*size].count, (*data)[*size].cost, (*data)[*size].primary, (*data)[*size].mode);
#endif
        (*size)++;
    }

#ifdef DUMP_ARRAYS
    printf("|---------------------------------|\n\n");
#endif

    fclose(fileOut);

    return SUCCESS;
}

/**
 * @brief Comparing function by id field. Needed for Timsort
 */
static inline int compareId(const void *a, const void *b)
{
    const StatData da = *((const StatData *) a);
    const StatData db = *((const StatData *) b);
    return (da.id < db.id) ? -1 : (da.id == db.id) ? 0 : 1;
}

int JoinDump(StatData *dataA, int sizeA, StatData *dataB, int sizeB, StatData **dataOut, int* sizeOut)
{
    int i, j = 0;

    /* Allocation memory for result array */
    *dataOut = malloc((sizeA + sizeB) * sizeof(StatData));

    int err;

    /* Sorting dataA array by id field */
    err = timsort(dataA, sizeA, sizeof(dataA[0]), compareId);
    if (err) {
        fprintf(stderr, "%s:Timsort returned error %d\n", __FUNCTION__, err);
    }
#ifdef DUMP_ARRAYS
    else {
        printf("|------------%s sorted 1st array by id----------|\n", __FUNCTION__);
        for (int i = 0; i < sizeA; ++i) {
            printf("|%d:| %ld | %d | %f | %u | %u |\n", i, dataA[i].id, dataA[i].count, dataA[i].cost, dataA[i].primary, dataA[i].mode);
        }
        printf("|---------------------------------|\n\n");
    }
#endif

    /* Sorting dataB array by id field */
    err = timsort(dataB, sizeB, sizeof(dataB[0]), compareId);
    if (err) {
        fprintf(stderr, "%s:Timsort returned error %d\n", __FUNCTION__, err);
    }
#ifdef DUMP_ARRAYS
    else {
        printf("|------------%s sorted 2nd array by id----------|\n", __FUNCTION__);
        for (int i = 0; i < sizeB; ++i) {
            printf("|%d:| %ld | %d | %f | %u | %u |\n", i, dataB[i].id, dataB[i].count, dataB[i].cost, dataB[i].primary, dataB[i].mode);
        }
        printf("|---------------------------------|\n\n");
    }
#endif

    /* Merging two sorted arrays */
    while (i < sizeA && j < sizeB) {
        if (dataA[i].id < dataB[j].id) {
            /* If it elem with new id then add it to result array */
            if (*sizeOut == 0 || (*dataOut)[*sizeOut - 1].id != dataA[i].id) {
                (*dataOut)[*sizeOut].id = dataA[i].id;
                (*dataOut)[*sizeOut].count = dataA[i].count;
                (*dataOut)[*sizeOut].cost = dataA[i].cost;
                (*dataOut)[*sizeOut].primary = dataA[i].primary;
                (*dataOut)[*sizeOut].mode = dataA[i].mode;
                (*sizeOut)++;
            }
            /* If result array contain elem with same id then merge elems */
            else if ((*dataOut)[*sizeOut - 1].id == dataA[i].id) {
                (*dataOut)[*sizeOut - 1].id = dataA[i].id;
                (*dataOut)[*sizeOut - 1].count = (*dataOut)[*sizeOut - 1].count + dataA[i].count;
                (*dataOut)[*sizeOut - 1].cost = (*dataOut)[*sizeOut - 1].cost + dataA[i].cost;
                (*dataOut)[*sizeOut - 1].primary = ((*dataOut)[*sizeOut - 1].primary == 0) ? (*dataOut)[*sizeOut - 1].primary : dataA[i].primary;
                (*dataOut)[*sizeOut - 1].mode = MAX((*dataOut)[*sizeOut - 1].mode, dataA[i].mode);
            }
            i++;
        } else if (dataA[i].id > dataB[j].id) {
            /* If it elem with new id then add it to result array */
            if (*sizeOut == 0 || (*dataOut)[*sizeOut - 1].id != dataB[j].id) {
                (*dataOut)[*sizeOut].id = dataB[j].id;
                (*dataOut)[*sizeOut].count = dataB[j].count;
                (*dataOut)[*sizeOut].cost = dataB[j].cost;
                (*dataOut)[*sizeOut].primary = dataB[j].primary;
                (*dataOut)[*sizeOut].mode = dataB[j].mode;
                (*sizeOut)++;
            }
            /* If result array contain elem with same id then merge elems */
            else if ((*dataOut)[*sizeOut - 1].id == dataB[j].id) {
                (*dataOut)[*sizeOut - 1].id = dataB[j].id;
                (*dataOut)[*sizeOut - 1].count = (*dataOut)[*sizeOut - 1].count + dataB[j].count;
                (*dataOut)[*sizeOut - 1].cost = (*dataOut)[*sizeOut - 1].cost + dataB[j].cost;
                (*dataOut)[*sizeOut - 1].primary = ((*dataOut)[*sizeOut - 1].primary == 0) ? (*dataOut)[*sizeOut - 1].primary : dataB[j].primary;
                (*dataOut)[*sizeOut - 1].mode = MAX((*dataOut)[*sizeOut - 1].mode, dataB[j].mode);
            }
            j++;
        } else {
            /* dataA[i].id == dataB[j].id case */
            /* If it elem with new id then add it to result array */
            if (*sizeOut == 0 || (*dataOut)[*sizeOut - 1].id != dataA[i].id) {
                (*dataOut)[*sizeOut].id = dataA[i].id;
                (*dataOut)[*sizeOut].count = dataA[i].count + dataB[j].count;
                (*dataOut)[*sizeOut].cost = dataA[i].cost + dataB[j].cost;
                (*dataOut)[*sizeOut].primary = (dataA[i].primary == 0) ? dataA[i].primary : dataB[j].primary;
                (*dataOut)[*sizeOut].mode = MAX(dataA[i].mode, dataB[j].mode);
                (*sizeOut)++;
            }
            /* If result array contain elem with same id then merge elems */
            else {
                (*dataOut)[*sizeOut - 1].id = dataA[i].id;
                (*dataOut)[*sizeOut - 1].count = (*dataOut)[*sizeOut - 1].count + dataA[i].count + dataB[j].count;
                (*dataOut)[*sizeOut - 1].cost = (*dataOut)[*sizeOut - 1].cost + dataA[i].cost + dataB[j].cost;
                (*dataOut)[*sizeOut - 1].primary = ((*dataOut)[*sizeOut - 1].primary == 0) ? (*dataOut)[*sizeOut - 1].primary : (dataA[i].primary == 0) ? dataA[i].primary : dataB[j].primary;
                (*dataOut)[*sizeOut - 1].mode = MAX(MAX((*dataOut)[*sizeOut - 1].mode, dataA[i].mode), dataB[j].mode);
            }
            i++;
            j++;
        }
    }

    /* Adding remaining elements from dataA[i] */
    while (i < sizeA) {
        /* If it elem with new id then add it to result array */
        if (*sizeOut == 0 || (*dataOut)[*sizeOut - 1].id != dataA[i].id) {
            (*dataOut)[*sizeOut].id = dataA[i].id;
            (*dataOut)[*sizeOut].count = dataA[i].count;
            (*dataOut)[*sizeOut].cost = dataA[i].cost;
            (*dataOut)[*sizeOut].primary = dataA[i].primary;
            (*dataOut)[*sizeOut].mode = dataA[i].mode;
            (*sizeOut)++;
        }
        /* If result array contain elem with same id then merge elems */
        else if ((*dataOut)[*sizeOut - 1].id == dataA[i].id) {
            (*dataOut)[*sizeOut - 1].id = dataA[i].id;
            (*dataOut)[*sizeOut - 1].count = (*dataOut)[*sizeOut - 1].count + dataA[i].count;
            (*dataOut)[*sizeOut - 1].cost = (*dataOut)[*sizeOut - 1].cost + dataA[i].cost;
            (*dataOut)[*sizeOut - 1].primary = ((*dataOut)[*sizeOut - 1].primary == 0) ? (*dataOut)[*sizeOut - 1].primary : dataA[i].primary;
            (*dataOut)[*sizeOut - 1].mode = MAX((*dataOut)[*sizeOut - 1].mode, dataA[i].mode);
        }
        i++;
    }

    /* Adding remaining elements from dataB[j] */
    while (j < sizeB) {
        /* If it elem with new id then add it to result array */
        if (*sizeOut == 0 || (*dataOut)[*sizeOut - 1].id != dataB[j].id) {
            (*dataOut)[*sizeOut].id = dataB[j].id;
            (*dataOut)[*sizeOut].count = dataB[j].count;
            (*dataOut)[*sizeOut].cost = dataB[j].cost;
            (*dataOut)[*sizeOut].primary = dataB[j].primary;
            (*dataOut)[*sizeOut].mode = dataB[j].mode;
            (*sizeOut)++;
        }
        /* If result array contain elem with same id then merge elems */
        else if ((*dataOut)[*sizeOut - 1].id == dataB[j].id) {
            (*dataOut)[*sizeOut - 1].id = dataB[j].id;
            (*dataOut)[*sizeOut - 1].count = (*dataOut)[*sizeOut - 1].count + dataB[j].count;
            (*dataOut)[*sizeOut - 1].cost = (*dataOut)[*sizeOut - 1].cost + dataB[j].cost;
            (*dataOut)[*sizeOut - 1].primary = ((*dataOut)[*sizeOut - 1].primary == 0) ? (*dataOut)[*sizeOut - 1].primary : dataB[j].primary;
            (*dataOut)[*sizeOut - 1].mode = MAX((*dataOut)[*sizeOut - 1].mode, dataB[j].mode);
        }
        j++;
    }

#ifdef DUMP_ARRAYS 
    printf("|------------%s merged array----------|\n", __FUNCTION__);
    for (int i = 0; i < *sizeOut; ++i) {
        printf("|%d:| %ld | %d | %f | %u | %u |\n", i, (*dataOut)[i].id, (*dataOut)[i].count, (*dataOut)[i].cost, (*dataOut)[i].primary, (*dataOut)[i].mode);
    }
    printf("|---------------------------------|\n\n");
#endif

    return SUCCESS;
}

/**
 * @brief Comparing function by cost field. Needed for Timsort
 */
static inline int compareCost(const void *a, const void *b)
{
    const StatData da = *((const StatData *) a);
    const StatData db = *((const StatData *) b);
    return (da.cost < db.cost) ? -1 : (da.cost == db.cost) ? 0 : 1;
}

int SortDump(StatData *data, int size)
{
    int err;

    /* Sorting data array by cost field */
    err = timsort(data, size, sizeof(data[0]), compareCost);
    if (err) {
        fprintf(stderr, "%s:Timsort returned error %d\n", __FUNCTION__, err);
        return ERROR_TIMSORT_FAILURE;
    }
#ifdef DUMP_ARRAYS 
    else {
        printf("|------------%s sorted array by cost----------|\n", __FUNCTION__);
        for (int i = 0; i < size; ++i) {
            printf("|%d:| %ld | %d | %f | %u | %u |\n", i, data[i].id, data[i].count, data[i].cost, data[i].primary, data[i].mode);
        }
        printf("|---------------------------------|\n\n");
    }
#endif
    return SUCCESS;
}