#include <stdio.h>
#include "benchmark.h"





int main() {



    long total_time, averagePush,averagePop;
    //double totalTime = ((double)(totaltime_end - totaltime_start))/CLOCKS_PER_SEC;
    pushBenchmark(15,&averagePush,&total_time);
    popBenchmark(15, &averagePop,&total_time);
    printf("Average Push Time: %f Seconds\n",averagePush/ 1e9);
    printf("Average Pop Time: %f Sekunden\n", averagePop/1e9);
    printf("Total Time: %lu Nano Sekunden\n", total_time);
    printf("Total Time: %f Sekunden\n", (double) total_time / 1e9);
    printf("Total Time: %f Minuten\n", ((double) total_time/ (1e9 *60)));
    return 0;
    return 0;
}

