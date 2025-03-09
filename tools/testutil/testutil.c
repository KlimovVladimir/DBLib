#include <libdb.h>
#include <time.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, RED "%s:Wrong count of arguments\n" RESET, __FUNCTION__);
        return 1;
    }

    clock_t begin, end;
    int error;

    ////////////////////Case 1////////////////////
    /* Содержимое для исходных файлов */
    const StatData case_1_in_a[2] = 
    {{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 }};
    const StatData case_1_in_b[2] = 
    {{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode=2 },
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode=2}};
    /* Ожидаемый результат обработки */
    const StatData case_1_out[3] = 
    {{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2 },
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3 },
    {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2 }};

    char pathCase1a[512] = { 0 };
    strcat(pathCase1a, argv[1]);
    strcat(pathCase1a, "case_1_in_a.txt");
    begin = clock();
    error = StoreDump(&pathCase1a, case_1_in_a, 2);
    end = clock();
    double StoreDumpTimeSpent1 = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:StoreDump case_1_in_a failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 1 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }

    char pathCase1b[512] = { 0 };
    strcat(pathCase1b, argv[1]);
    strcat(pathCase1b, "case_1_in_b.txt");
    begin = clock();
    error = StoreDump(&pathCase1b, case_1_in_b, 2);
    end = clock();
    double StoreDumpTimeSpent2 = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:StoreDump case_1_in_b failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 1 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }

    char pathCmd[2048] = { 0 };
    strcat(pathCmd, "dbutil ");
    strcat(pathCmd, pathCase1a);
    strcat(pathCmd, " ");
    strcat(pathCmd, pathCase1b);
    strcat(pathCmd, " ");
    strcat(pathCmd, argv[1]);
    strcat(pathCmd, "case_1_out.txt");
    begin = clock();
    error = system(pathCmd);
    end = clock();
    double dbutilTimeSpent = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:dbutil failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 1 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }
    
    StatData *dataA;
    int sizeA = 0;
    char pathCase1Out[512] = { 0 };
    strcat(pathCase1Out, argv[1]);
    strcat(pathCase1Out, "case_1_out.txt");
    begin = clock();
    error = LoadDump(&pathCase1Out, &dataA, &sizeA);
    end = clock();
    double LoadDumpTimeSpent = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:LoadDump failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 1 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }

    int passed = 1;
    for (int i = 0; i < sizeA; ++i) {
        if (dataA[i].id != case_1_out[i].id || dataA[i].count != case_1_out[i].count
        || dataA[i].cost != case_1_out[i].cost || dataA[i].primary != case_1_out[i].primary
        || dataA[i].mode != case_1_out[i].mode) {
            passed = 0;
            fprintf(stderr, RED "%s:Case 1 not passed\n" RESET, __FUNCTION__);
            break;
        }
    }

    free(dataA);

    if (passed) {
        fprintf(stdout, GRN "%s:Case 1 passed\n" RESET, __FUNCTION__);
        fprintf(stdout, "%s:StoreDumpTimeSpent1: %lf\n", __FUNCTION__, StoreDumpTimeSpent1);
        fprintf(stdout, "%s:StoreDumpTimeSpent2: %lf\n", __FUNCTION__, StoreDumpTimeSpent2);
        fprintf(stdout, "%s:dbutilTimeSpent: %lf\n", __FUNCTION__, dbutilTimeSpent);
        fprintf(stdout, "%s:LoadDumpTimeSpent: %lf\n", __FUNCTION__, LoadDumpTimeSpent);
    }

endCase1:

    ////////////////////Case 2////////////////////
    /* Содержимое для исходных файлов */
    const StatData case_2_in_a[10] = 
    {{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=1 },
    {.id = 123, .count = 1000, .cost = 1.567, .primary = 0, .mode=3 },
    {.id = 3234, .count = 13, .cost = 3.12321, .primary = 0, .mode=3 },
    {.id = 234, .count = 555, .cost = 33333.567, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 0.567, .primary = 0, .mode=1 },
    {.id = 234, .count = 777, .cost = 3.0001, .primary = 0, .mode=3 },
    {.id = 123, .count = 13, .cost = 1.001, .primary = 0, .mode=3 },
    {.id = 90889, .count = 0, .cost = 3.567, .primary = 0, .mode=5 },
    {.id = 90889, .count = 13, .cost = 1212.4354324, .primary = 0, .mode=2 },
    {.id = 234, .count = 1, .cost = 88.90, .primary = 1, .mode=7 }};
    const StatData case_2_in_b[30] = 
    {{.id = 1, .count = 13, .cost = 3.567, .primary = 0, .mode=1 },
    {.id = 123, .count = 1000, .cost = 1.567, .primary = 0, .mode=3 },
    {.id = 3234, .count = 13, .cost = 3.12321, .primary = 0, .mode=3 },
    {.id = 234, .count = 555, .cost = 33333.567, .primary = 0, .mode=3 },
    {.id = 2, .count = 13, .cost = 0.567, .primary = 1, .mode=1 },
    {.id = 234, .count = 777, .cost = 3.0001, .primary = 0, .mode=3 },
    {.id = 123, .count = 13, .cost = 1.001, .primary = 0, .mode=3 },
    {.id = 90889, .count = 0, .cost = 3.567, .primary = 0, .mode=5 },
    {.id = 3, .count = 13, .cost = 1212.4354324, .primary = 0, .mode=2 },
    {.id = 234, .count = 1, .cost = 88.90, .primary = 1, .mode=7 },
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=1 },
    {.id = 123, .count = 1000, .cost = 1.567, .primary = 0, .mode=3 },
    {.id = 3, .count = 13, .cost = 3.12321, .primary = 0, .mode=3 },
    {.id = 234, .count = 555, .cost = 33333.567, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 0.567, .primary = 0, .mode=1 },
    {.id = 3, .count = 777, .cost = 3.0001, .primary = 0, .mode=3 },
    {.id = 123, .count = 13, .cost = 1.001, .primary = 0, .mode=3 },
    {.id = 2, .count = 0, .cost = 3.567, .primary = 1, .mode=5 },
    {.id = 90889, .count = 13, .cost = 1212.4354324, .primary = 0, .mode=2 },
    {.id = 234, .count = 1, .cost = 88.90, .primary = 1, .mode=7 },
    {.id = 1, .count = 13, .cost = 3.567, .primary = 0, .mode=1 },
    {.id = 123, .count = 1000, .cost = 1.567, .primary = 0, .mode=3 },
    {.id = 3234, .count = 13, .cost = 3.12321, .primary = 0, .mode=3 },
    {.id = 234, .count = 555, .cost = 33333.567, .primary = 0, .mode=3 },
    {.id = 1, .count = 13, .cost = 0.567, .primary = 0, .mode=1 },
    {.id = 234, .count = 777, .cost = 3.0001, .primary = 0, .mode=3 },
    {.id = 123, .count = 13, .cost = 1.001, .primary = 0, .mode=3 },
    {.id = 1, .count = 0, .cost = 3.567, .primary = 0, .mode=5 },
    {.id = 90889, .count = 13, .cost = 1212.4354324, .primary = 0, .mode=2 },
    {.id = 1, .count = 1, .cost = 88.90, .primary = 1, .mode=7 }};
    /* Ожидаемый результат обработки */
    const StatData case_2_out[7] = 
    {{.id = 2, .count = 13, .cost = 4.134, .primary = 1, .mode = 5 },
    {.id = 3234, .count = 39, .cost = 9.36963, .primary = 0, .mode = 3 },
    {.id = 123, .count = 4052, .cost = 10.272, .primary = 0, .mode = 3 },
    {.id = 1, .count = 40, .cost = 100.167999, .primary = 0, .mode = 7 },
    {.id = 3, .count = 803, .cost = 1218.558716, .primary = 0, .mode = 3 },
    {.id = 90889, .count = 91, .cost = 3652.708496, .primary = 0, .mode = 5 },
    {.id = 234, .count = 4554, .cost = 133609.968750, .primary = 0, .mode = 7 }};

    char pathCase2a[512] = { 0 };
    strcat(pathCase2a, argv[1]);
    strcat(pathCase2a, "case_2_in_a.txt");
    begin = clock();
    error = StoreDump(&pathCase2a, case_2_in_a, 10);
    end = clock();
    double StoreDumpTimeSpent21 = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:StoreDump case_2_in_a failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 2 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }

    char pathCase2b[512] = { 0 };
    strcat(pathCase2b, argv[1]);
    strcat(pathCase2b, "case_2_in_b.txt");
    begin = clock();
    error = StoreDump(&pathCase2b, case_2_in_b, 30);
    end = clock();
    double StoreDumpTimeSpent22 = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:StoreDump case_2_in_b failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 2 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }

    char pathCmd2[2048] = { 0 };
    strcat(pathCmd2, "dbutil ");
    strcat(pathCmd2, pathCase2a);
    strcat(pathCmd2, " ");
    strcat(pathCmd2, pathCase2b);
    strcat(pathCmd2, " ");
    strcat(pathCmd2, argv[1]);
    strcat(pathCmd2, "case_2_out.txt");
    begin = clock();
    error = system(pathCmd2);
    end = clock();
    double dbutilTimeSpent2 = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:dbutil failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 2 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }
    
    StatData *dataA2;
    int sizeA2 = 0;
    char pathCase2Out[512] = { 0 };
    strcat(pathCase2Out, argv[1]);
    strcat(pathCase2Out, "case_2_out.txt");
    begin = clock();
    error = LoadDump(&pathCase2Out, &dataA2, &sizeA2);
    end = clock();
    double LoadDumpTimeSpent2 = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:LoadDump failed. Error:%d\n" RESET, __FUNCTION__, error);
        fprintf(stderr, RED "%s:Case 2 not passed\n" RESET, __FUNCTION__);
        goto endCase1;
    }

    int passed2 = 1;
    for (int i = 0; i < sizeA2; ++i) {
        if (dataA2[i].id != case_2_out[i].id || dataA2[i].count != case_2_out[i].count
        || dataA2[i].cost != case_2_out[i].cost || dataA2[i].primary != case_2_out[i].primary
        || dataA2[i].mode != case_2_out[i].mode) {
            passed = 0;
            fprintf(stderr, RED "%s:Case 1 not passed\n" RESET, __FUNCTION__);
            break;
        }
    }

    free(dataA2);

    if (passed2) {
        fprintf(stdout, GRN "%s:Case 2 passed\n" RESET, __FUNCTION__);
        fprintf(stdout, "%s:StoreDumpTimeSpent1: %lf\n", __FUNCTION__, StoreDumpTimeSpent21);
        fprintf(stdout, "%s:StoreDumpTimeSpent2: %lf\n", __FUNCTION__, StoreDumpTimeSpent22);
        fprintf(stdout, "%s:dbutilTimeSpent: %lf\n", __FUNCTION__, dbutilTimeSpent2);
        fprintf(stdout, "%s:LoadDumpTimeSpent: %lf\n", __FUNCTION__, LoadDumpTimeSpent2);
    }

endCase2:

    return 0;
}