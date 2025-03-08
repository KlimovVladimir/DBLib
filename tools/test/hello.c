#include <libdb.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

// /* Used to control the stress test */
// #define SEED 123
// #define MAXSIZE 10000
// #define TESTS 1
// #define TYPE int

// /* helper functions */
// void verify(TYPE *dst, const size_t size)
// {
//   size_t i;
//   for (i = 1; i < size; i++)
//   {
//     if (dst[i - 1] > dst[i])
//     {
// 	    printf("Verify failed! at %zd\n", i);
// 	    //for (i = i - 2; i < size; i++)
// 	    //  printf(" %lld", dst[i]);
// 	    //printf("\n");
// 	    break;
//     }
//   }
// }

// static void fill(TYPE *dst, const size_t size)
// {
//   size_t i;
//   for (i = 0; i < size; i++)
//   {
//     dst[i] = lrand48();
//     printf(" %d|", dst[i]);
//   }
// }

// static int compare(const void *a, const void *b)
// {
//   const TYPE da = *((const TYPE *) a);
//   const TYPE db = *((const TYPE *) b);
//   return (da < db) ? -1 : (da == db) ? 0 : 1;
// }

// static int compare_arg(const void *a, const void *b, void *arg)
// {
//   const TYPE da = *((const TYPE *) a);
//   const TYPE db = *((const TYPE *) b);
//   return (da < db) ? -1 : (da == db) ? 0 : 1;
// }


// void run_tests(void)
// {
// 	int err;
// 	int test;
// 	TYPE *dst;
// 	size_t size;

// 	printf("Running tests\n");

// #if 1
// 	printf("timsort\n");
// 	for (test = 0; test < TESTS; test++)
// 	{
// 		size = (20);
//         printf("size %d| \n", size);
// 		dst = malloc(size * sizeof(dst[0]));
// 		if (!dst && size) {
// 			perror("malloc failed");
// 			exit(EXIT_FAILURE);
// 		}

// 		fill(dst, size);

//         /////////////////
//         for (int i = 0; i < size; i++)
//         {
//             printf(" %d|", dst[i]);
//         }
//         printf("\n\n\n\n");
//         /////////////////
// #ifdef USE_CMP_ARG
// 		err = timsort_r(dst, size, sizeof(dst[0]), compare_arg, NULL);
// #else
// 		err = timsort(dst, size, sizeof(dst[0]), compare);
// #endif
// 		if (err) {
// 			perror("timsort failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		verify(dst, size);
// #ifdef USE_CMP_ARG
// 		err = timsort_r(dst, size, sizeof(dst[0]), compare_arg, NULL);
// #else
// 		err = timsort(dst, size, sizeof(dst[0]), compare);
// #endif
// 		if (err) {
// 			perror("timsort failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		verify(dst, size);

//         /////////////////
//         for (int i = 0; i < size; i++)
//         {
//             printf(" %d|", dst[i]);
//         }
//         printf("\n");
//         /////////////////

// 		free(dst);
// 	}
//     printf("timsort success\n");
// #else
// 	printf("mergesort\n");
// 	for (test = 0; test < TESTS; test++)
// 	{
// 		size = (lrand48() % (MAXSIZE + 1));
// 		dst = malloc(size * sizeof(dst[0]));
// 		if (!dst && size) {
// 			perror("malloc failed");
// 			exit(EXIT_FAILURE);
// 		}

// 		fill(dst, size);
// 		err = mergesort(dst, size, sizeof(dst[0]), compare);
// 		if (err) {
// 			perror("mergesort failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		verify(dst, size);
// 		err = mergesort(dst, size, sizeof(dst[0]), compare);
// 		if (err) {
// 			perror("mergesort failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		verify(dst, size);
		
// 		free(dst);
// 	} 
// #endif
// }

// int main(void)
// {
//     hello();
// 	run_tests();
// 	return EXIT_SUCCESS;
// }





int unsorted[] = {25,29,16,30,5,28,27,9,13,23,26,2,29,3,11,17,7,0,21,19,24,4,8,22,12,10,18,15,20,14,6,1,31};

typedef int TYPE ;
static inline int compare(const void *a, const void *b)
{
  const TYPE da = *((const TYPE *) a);
  const TYPE db = *((const TYPE *) b);
  /* DECREASING sort */
  return (da < db) ? -1 : (da == db) ? 0 : 1;
}

int main(int argc, char *argv[])
{
    int err, i;
    char *sep = "";

    hello();

//   err = timsort(unsorted, sizeof(unsorted)/sizeof(unsorted[0]), sizeof(unsorted[0]), compare);
//   if (err) {
//     fprintf(stderr, "timsort returned error %d\n", err);
//   } else {
//     for (i =0; i < (sizeof(unsorted)/sizeof(unsorted[0])); ++i) {
//       fprintf(stdout, "%s%d", sep, unsorted[i]);
//       sep = " ";
//     }
//   }
  /* Содержимое для исходных файлов */
    const StatData case_1_in_a[10] = 
    {{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 123, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 3234, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 234, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 234, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 123, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 234, .count = 1, .cost = 88.90, .primary = 1, .mode=0 }};
    //StoreDump("/home/vladimir/dbUtils/123/test.txt", &case_1_in_a, 2);

    for (int i = 0; i < 10; i++)
        fprintf(stderr, "| %ld | %d | %f | %u | %u |\n", case_1_in_a[i].id, case_1_in_a[i].count, case_1_in_a[i].cost, case_1_in_a[i].primary, case_1_in_a[i].mode);



    StatData case_1_in_b[10];
    int size = LoadDump("/home/vladimir/dbUtils/123/test.txt", &case_1_in_b);
    fprintf(stderr, "size %d\n", size);

    //SortDump(&case_1_in_b, size);

    JoinDump(case_1_in_a, 10, case_1_in_b, 10, case_1_in_b, 10);

    //for (int i = 0; i < 2; i++)
    //    fprintf(stderr, "%ld %d %f %u %u\n", case_1_in_b[i].id, case_1_in_b[i].count, case_1_in_b[i].cost, case_1_in_b[i].primary, case_1_in_b[i].mode);

    return 0;
}