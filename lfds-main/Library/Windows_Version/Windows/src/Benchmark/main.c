
#include "../header/benchmark.h"
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    /*
     * normal compiling
     */


    int ds = atoi(argv[1]);
    int threads = atoi(argv[2]);
    int insertion = atoi(argv[3]);
    int deletion = atoi(argv[4]);

    if (ds == 1) {
        printf("QUEUE:\n");
        lock_free_benchmark(QUEUE, threads, insertion, deletion);
        lock_based_benchmark(QUEUE, threads, insertion, deletion);
    } else if (ds == 2) {
        printf("STACK:\n");
        lock_free_benchmark(STACK, threads, insertion, deletion);
        lock_based_benchmark(STACK, threads, insertion, deletion);
    } else if (ds == 3) {
        printf("SET:\n");
        lock_free_benchmark(SET, threads, insertion, deletion);
        lock_based_benchmark(SET, threads, insertion, deletion);
    } else
        printf("Fehler in Main bei der Übergabe Parameter");
    return 0;


}




