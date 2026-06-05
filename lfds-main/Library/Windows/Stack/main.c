
#include "benchmark.h"


  int main() {
      lock_free_benchmark(STACK,2,1000);
      //lock_based_benchmark(STACK,1,100);
      return 0;
  }

