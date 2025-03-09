#include <libdb.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"

int main(int argc, char *argv[])
{
    const StatData case_1_in_a[10] = 
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

    int error;

    error = StoreDump(argv[1], &case_1_in_a, 10);

    if (error != SUCCESS) {
        fprintf(stderr, RED "%s:StoreDump failed. Error:%d\n" RESET, __FUNCTION__, error);
        return 1;
    }

    return 0;
}