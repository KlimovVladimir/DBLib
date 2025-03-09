#include <stdio.h>
#include <stdlib.h>
#include "timsort.h"

/* Flag for enabling array dumping after operations with them */
#define DUMP_ARRAYS

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Functions return codes */
#define SUCCESS 0
#define ERROR_OPEN_FILE -1
#define ERROR_INVALID_VALUE -2
#define ERROR_REALLOC_FAILURE -3
#define ERROR_TIMSORT_FAILURE -4

/* Elem of array */
typedef struct StatData {
    long id;
    int count;
    float cost;
    unsigned int primary:1;
    unsigned int mode:3;
} StatData;

/**
 * @brief Function for saving array to the file.
 * @param pathOutFile path to the file where array will be saved.
 * @param data array which will be saved.
 * @param size size of data array.
 * @return 0 if it success. Otherwise return error code
 */
int StoreDump(char *pathOutFile, StatData *data, int size);

/**
 * @brief Function for reading array from the file.
 * @param pathOutFile path to the file with data.
 * @param data pointer where array will be placed.
 * @param size size of data array.
 * @return 0 if it success. Otherwise return error code
 */
int LoadDump(char *pathOutFile, StatData **data, int* size);

/**
 * @brief Function for merging two arrays.
 * @param dataA 1st array.
 * @param sizeA 1st array size.
 * @param dataB 2st array.
 * @param sizeB 2st array size.
 * @param dataOut result array.
 * @param sizeOut result array size.
 * @return 0 if it success. Otherwise return error code
 */
int JoinDump(StatData *dataA, int sizeA, StatData *dataB, int sizeB, StatData **dataOut, int *sizeOut);

/**
 * @brief Function for sorting array by cost field. Timsort algorithm is used.
 * @param data array which will be sorted.
 * @param size size of data array.
 * @return 0 if it success. Otherwise return error code
 */
int SortDump(StatData *data, int size);