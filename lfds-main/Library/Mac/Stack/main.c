
#include "benchmark.h"


  int main() {
      lock_free_benchmark(STACK,2,500,500);
      //lock_based_benchmark(STACK,1,100);
      return 0;
  }

